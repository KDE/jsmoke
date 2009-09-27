/*
 *   Copyright 2008-2009 by Richard Dale <richard.j.dale@gmail.com>

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

namespace QtScript {

MethodCall::MethodCall(Smoke *smoke, Smoke::Index method, QScriptContext * context, QScriptEngine * engine) :
    m_current(-1), m_smoke(smoke), m_method(method), m_context(context), m_engine(engine), m_called(false)
{
    m_target = m_context->thisObject();
    if (QtScript::SmokeInstance::isSmokeObject(m_target)) {
        m_instance = QtScript::SmokeInstance::get(m_target);
    } else {
        m_instance = 0;
    }
    
    m_args = m_smoke->argumentList + m_smoke->methods[m_method].args;
    m_items = m_smoke->methods[m_method].numArgs;
    m_stack = new Smoke::StackItem[m_items + 1];
}

MethodCall::~MethodCall() 
{ 
    delete[] m_stack; 
}

void MethodCall::unsupported()
{
    if (strcmp(m_smoke->className(method().classId), "QGlobalSpace") == 0) {
        qFatal("Cannot handle '%s' as argument to %s",
               type().name(),
               m_smoke->methodNames[method().name]);
    } else {
        qFatal("Cannot handle '%s' as argument to %s::%s",
               type().name(),
               m_smoke->className(method().classId),
               m_smoke->methodNames[method().name]);
    }
}

void MethodCall::callMethod()
{
    if (m_called) {
        return;
    }
    
    m_called = true;
    Smoke::ClassFn fn = m_smoke->classes[method().classId].classFn;
    void *ptr = 0;

    if (m_instance != 0 && m_instance->value != 0) {
        const Smoke::Class &cl = m_smoke->classes[method().classId];

        ptr = m_instance->classId.smoke->cast(  m_instance->value,
                                                m_instance->classId.index,
                                                m_instance->classId.smoke->idClass(cl.className, true).index );
    }
    m_items = -1;
    
    (*fn)(method().method, ptr, m_stack);
    
    if (isConstructor()) {
        Smoke::StackItem stack[2];
        // stack[1].s_class = qscript_modules[m_smoke].binding;
        fn(0, m_stack[0].s_voidp, stack);
        // m_instance = allocm_smokeqyotom_instancebject(true, m_smoke, method().classId, m_stack[0].s_voidp);
        // (*SetSmokeObject)(m_target, m_instance);
        // mapPointer(m_target, m_instance, m_instance->classId, 0);
    } else if (isDestructor()) {
    } else {
        QtScript::MethodReturnValue result(m_smoke, m_method, m_stack, m_engine);
    }
}

void MethodCall::next()
{
    int previous = m_current;
    m_current++;
    
    while (!m_called && m_current < m_items) {
        Marshall::HandlerFn fn = getMarshallFn(type());
        (*fn)(this);
        m_current++;
    }
    
    callMethod();
    m_current = previous;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
