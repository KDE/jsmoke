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
// #include "methodreturnvalue.h"
#include "smokebinding.h"
#include "global.h"

#include "smoke/qtcore_smoke.h"

#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContextInfo>

namespace JSmoke {
    
MethodCall::ArgumentTypeConversion::ArgumentTypeConversion(Smoke::ModuleIndex methodId, Smoke::StackItem& item, QScriptValue& value) :
    m_methodId(methodId), m_item(item), m_value(value) 
{
    m_stack = new Smoke::StackItem[method().numArgs + 1];
    QByteArray methodName(m_methodId.smoke->methodNames[method().name]);
    
    if (methodName.startsWith("operator ")) {
        m_type.set(m_methodId.smoke, method().ret);
    } else {
        m_type.set(m_methodId.smoke, (m_methodId.smoke->argumentList + method().args)[0]);
    }
    
    Marshall::HandlerFn handlerFn = getMarshallFn(type());
    (*handlerFn)(this);
    
    // Note that the method called can be in one of two forms:
    //      1) A constructor 'Foobar::Foobar(value)',
    //      2) A 'value::operator Foobar()' method. 
    // In case 1) the argument is in m_stack[1], and the result in m_stack[0]
    // In case 2) the instance to call the method on, is m_stack[1].s_voidp,
    // and the result is in m_stack[0].
    Smoke::ClassFn fn = m_methodId.smoke->classes[method().classId].classFn;
    (*fn)(method().method, m_stack[1].s_voidp, m_stack);
    m_item = m_stack[0];
}

MethodCall::ArgumentTypeConversion::~ArgumentTypeConversion()
{
    QByteArray methodName(m_methodId.smoke->methodNames[method().name]);
    QByteArray className;
    Smoke::ModuleIndex classId;
    
    if (methodName.startsWith("operator ")) {
        className = methodName.mid(qstrlen("operator "));
        classId = Smoke::findClass(className);
    } else {
        className = m_methodId.smoke->classes[method().classId].className;
        classId = Smoke::ModuleIndex(m_methodId.smoke, method().classId);
    }
    
    QByteArray destructorName(className);
    destructorName.prepend("~");
    Smoke::ModuleIndex nameId = classId.smoke->findMethodName(className, destructorName);
    Smoke::ModuleIndex methodId = classId.smoke->findMethod(classId, nameId);
    
    if (methodId != Smoke::NullModuleIndex) {
        Smoke::Method &methodRef = classId.smoke->methods[classId.smoke->methodMaps[methodId.index].method];
        Smoke::ClassFn fn = classId.smoke->classes[methodRef.classId].classFn;
        m_stack[1] = m_stack[0];
        (*fn)(methodRef.method, m_stack[1].s_voidp, m_stack);
    }
    
    delete[] m_stack; 
}

void
MethodCall::ArgumentTypeConversion::unsupported()
{
    m_value.engine()->currentContext()->throwError( QScriptContext::TypeError, 
                                                    QString("Cannot handle '%1' as value type conversion %2::%3")
                                                            .arg(type().name())
                                                            .arg(m_methodId.smoke->className(method().classId))
                                                            .arg(m_methodId.smoke->methodNames[method().name]) );
}

void
MethodCall::ArgumentTypeConversion::next() 
{
}

MethodCall::ReturnValue::ReturnValue(Smoke::ModuleIndex methodId, Smoke::Stack stack, QScriptEngine * engine, QScriptValue * returnValue) :
    m_methodId(methodId), m_stack(stack), m_engine(engine), m_returnValue(returnValue)
{
    Marshall::HandlerFn fn = getMarshallFn(type());
    (*fn)(this);
}

void
MethodCall::ReturnValue::unsupported()
{
    if (strcmp(m_methodId.smoke->className(method().classId), "QGlobalSpace") == 0) {
        m_engine->currentContext()->throwError( QScriptContext::TypeError, 
                                                QString("Cannot handle '%1' as return type of %2")
                                                        .arg(type().name())
                                                        .arg(m_methodId.smoke->methodNames[method().name]) );
    } else {
        m_engine->currentContext()->throwError( QScriptContext::TypeError, 
                                                QString("Cannot handle '%1' as return type of %2::%3")
                                                        .arg(type().name())
                                                        .arg(m_methodId.smoke->className(method().classId))
                                                        .arg(m_methodId.smoke->methodNames[method().name]) );
    }
}

void
MethodCall::ReturnValue::next() 
{
}

MethodCall::MethodCall( const QVector<Smoke::ModuleIndex>& methodIds, 
                        QScriptContext * context, 
                        QScriptEngine * engine, 
                        QScriptValueList& args ) :
    m_methodIds(methodIds), m_methodRef(methodIds[0].smoke->methods[methodIds[0].index]), 
    m_current(-1), m_context(context), m_engine(engine), 
    m_valueList(args), m_called(false), m_error(false)
{
    m_methodId = m_methodIds[0]; 
    m_smoke = m_methodId.smoke;
    m_target = m_context->thisObject();
    m_instance = JSmoke::Object::Instance::get(m_target); 
    m_args = m_smoke->argumentList + m_methodRef.args;
    m_stack = new Smoke::StackItem[m_methodRef.numArgs + 1];
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
        ptr = m_instance->classId.smoke->cast(  m_instance->value, 
                                                m_instance->classId, 
                                                Smoke::ModuleIndex(m_smoke, m_methodRef.classId) );
    }
    
    (*fn)(m_methodRef.method, ptr, m_stack);
    
    if ((m_methodRef.flags & Smoke::mf_ctor) != 0) {
        Smoke::StackItem initializeInstanceStack[2];
        initializeInstanceStack[1].s_voidp = Global::modules[m_smoke].binding;
        fn(0, m_stack[0].s_class, initializeInstanceStack);
        
        m_returnValue = m_context->thisObject();
        
        bool isQObject = qtcore_Smoke->isDerivedFrom(   Smoke::ModuleIndex(m_smoke, m_methodRef.classId),
                                                        Global::QObjectClassId );

        if (isQObject) {
            m_instance = new SmokeQObject::Instance();
            QObject * obj = static_cast<QObject*>(m_smoke->cast(    m_stack[0].s_class, 
                                                                    Smoke::ModuleIndex(m_smoke, m_methodRef.classId),
                                                                    Global::QObjectClassId ) );
            (static_cast<SmokeQObject::Instance*>(m_instance))->qobject = m_engine->newQObject(obj);
        } else {
            m_instance = new Object::Instance();
        }
                 
        m_instance->classId.smoke = m_smoke;
        m_instance->classId.index = m_methodRef.classId;
        m_instance->value = m_stack[0].s_class;
        m_instance->ownership = QScriptEngine::ScriptOwnership;
        
        Object::Instance::set(m_returnValue, m_instance);
        JSmoke::Global::mapPointer(new QScriptValue(m_context->thisObject()), m_instance, m_instance->classId);
    } else {
        m_returnValue = m_engine->undefinedValue();
        ReturnValue result(m_methodId, m_stack, m_engine, &m_returnValue);
    }
    
    if ((Debug::DoDebug & Debug::Calls) != 0) {
        QStringList args;
        foreach (QScriptValue arg, m_valueList) {
            args << arg.toString();
        }
        
        QScriptContextInfo contextInfo(m_context->parentContext());
        qWarning(   "Trace@%s:%d %s.%s(%s) => %s",
                    contextInfo.fileName().toLatin1().constData(),
                    contextInfo.lineNumber(),
                    m_smoke->className(m_methodRef.classId),
                    m_smoke->methodNames[m_methodRef.name], 
                    args.join(", ").toLatin1().constData(),
                    m_returnValue.toString().toLatin1().constData() );
    }
}

void MethodCall::next()
{
    int previous = m_current;
    m_current++;
    
    while (!m_called && !m_error && m_current < m_methodRef.numArgs) {
        if (hasTypeConversion()) {
            ArgumentTypeConversion conversion(typeConversion(), item(), *var());
            next();
        } else {
            Marshall::HandlerFn fn = getMarshallFn(type());
            (*fn)(this);
        }
        
        m_current++;
    }
    
    callMethod();
    m_current = previous;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
