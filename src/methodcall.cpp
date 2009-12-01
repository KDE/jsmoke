/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "methodcall.h"
#include "methodreturnvalue.h"
#include "smokebinding.h"
#include "global.h"

#include "smoke/qtcore_smoke.h"

#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>

namespace QtScriptSmoke {

MethodCall::MethodCall(Smoke *smoke, Smoke::Index method, QScriptContext * context, QScriptEngine * engine) :
    m_current(-1), m_smoke(smoke), m_method(method), m_context(context), m_engine(engine), 
    m_called(false), m_error(false),
    m_methodRef(smoke->methods[method])
{
    m_target = m_context->thisObject();
    m_instance = QtScriptSmoke::Object::Instance::get(m_target); 
    m_args = m_smoke->argumentList + m_methodRef.args;
    m_stack = new Smoke::StackItem[m_methodRef.numArgs + 1];
    
    // More thought needed - this is maybe a bit less efficient
    // than it oould be..
    for (int count = 0; count < m_methodRef.numArgs; count++) {
        m_valueList << m_context->argument(count);
    }
}

MethodCall::~MethodCall() 
{ 
    delete[] m_stack; 
}

void MethodCall::unsupported()
{
    QString message;
    
    if (qstrcmp(m_smoke->className(m_methodRef.classId), "QGlobalSpace") == 0) {
        message = QString("Cannot handle '%1' as argument to %2")
                            .arg(type().name())
                            .arg(m_smoke->methodNames[m_methodRef.name]);
    } else {
        message = QString("Cannot handle '%1' as argument to %2::%3")
                            .arg(type().name())
                            .arg(m_smoke->className(m_methodRef.classId))
                            .arg(m_smoke->methodNames[m_methodRef.name]);
    }
    
    m_returnValue = m_context->throwError(QScriptContext::TypeError, message);
    m_error = true;
}

void MethodCall::callMethod()
{
    if (m_called || m_error) {
        return;
    }
    
    m_called = true;
    Smoke::ClassFn fn = m_smoke->classes[m_methodRef.classId].classFn;
    void *ptr = 0;

    if (m_instance != 0 && m_instance->value != 0) {
        if (m_instance->classId.smoke == m_smoke) {
            ptr = m_instance->classId.smoke->cast(  m_instance->value,
                                                    m_instance->classId.index,
                                                    m_methodRef.classId );
        } else {
            // If the method's class and the instance's class are in different smoke modules
            // then we need to convert them both to be class ids in the instance's module in
            // order to do the cast
            const Smoke::Class &cl = m_smoke->classes[m_methodRef.classId];
            ptr = m_instance->classId.smoke->cast(  m_instance->value,
                                                    m_instance->classId.index,
                                                    m_instance->classId.smoke->idClass(cl.className, true).index );
        }
    }
    
    (*fn)(m_methodRef.method, ptr, m_stack);
    
    if ((m_methodRef.flags & Smoke::mf_ctor) != 0) {
        Smoke::StackItem initializeInstanceStack[2];
        initializeInstanceStack[1].s_voidp = Global::modules[m_smoke].binding;
        fn(0, m_stack[0].s_class, initializeInstanceStack);
        
        m_returnValue = m_context->thisObject();
        
        bool isQObject = qtcore_Smoke->isDerivedFrom(   m_smoke, 
                                                        m_methodRef.classId,
                                                        Global::QObjectClassId.smoke,
                                                        Global::QObjectClassId.index );
        if (isQObject) {
            m_instance = new SmokeQObject::Instance();
            QObject * obj = static_cast<QObject*>(m_smoke->cast(    m_stack[0].s_class, 
                                                                    m_methodRef.classId, 
                                                                    Global::QObjectClassId.index ) );
            (static_cast<SmokeQObject::Instance*>(m_instance))->qobject = m_engine->newQObject(obj);
        } else {
            m_instance = new Object::Instance();
        }
                 
        m_instance->classId.smoke = m_smoke;
        m_instance->classId.index = m_methodRef.classId;
        m_instance->value = m_stack[0].s_class;
        m_instance->ownership = QScriptEngine::ScriptOwnership;
        
        Object::Instance::set(m_returnValue, m_instance);
        QtScriptSmoke::Global::mapPointer(new QScriptValue(m_context->thisObject()), m_instance, m_instance->classId.index, 0);
    } else {
        m_returnValue = m_engine->undefinedValue();
        QtScriptSmoke::MethodReturnValue result(m_smoke, m_method, m_stack, m_engine, &m_returnValue);
    }
    
    if ((Debug::DoDebug & Debug::Calls) != 0) {
        QStringList args;
        foreach (QScriptValue arg, m_valueList) {
            args << arg.toString();
        }
        
        QScriptValue result = (m_methodRef.flags & Smoke::mf_ctor) != 0 ? m_context->thisObject() : m_returnValue;
        qWarning(   "Trace: %s.%s(%s) => %s", 
                    m_smoke->className(m_methodRef.classId),
                    m_smoke->methodNames[m_methodRef.name], 
                    args.join(", ").toLatin1().constData(),
                    result.toString().toLatin1().constData() );
    }
}

void MethodCall::next()
{
    int previous = m_current;
    m_current++;
    
    while (!m_called && !m_error && m_current < m_methodRef.numArgs) {
        Marshall::HandlerFn fn = getMarshallFn(type());
        (*fn)(this);
        m_current++;
    }
    
    callMethod();
    m_current = previous;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
