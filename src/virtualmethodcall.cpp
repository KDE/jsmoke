/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   Based on the PerlQt marshalling code by Ashley Winters

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

#include "virtualmethodcall.h"
#include "virtualmethodreturnvalue.h"

namespace JSmoke {
    
VirtualMethodCall::ReturnValue::ReturnValue(Smoke *smoke, Smoke::Index meth, Smoke::Stack stack, QScriptValue returnValue) :
    m_smoke(smoke), m_method(meth), m_stack(stack), m_returnValue(returnValue) 
{
    m_type.set(m_smoke, method().ret);
    Marshall::HandlerFn fn = getMarshallFn(type());
    (*fn)(this);
}

void
VirtualMethodCall::ReturnValue::unsupported()
{
    m_returnValue.engine()->currentContext()->throwError(   QScriptContext::TypeError, 
                                                            QString("Cannot handle '%1' as return type of virtual method %2::%3")
                                                                    .arg(type().name())
                                                                    .arg(m_smoke->className(method().classId))
                                                                    .arg(m_smoke->methodNames[method().name]) );
}

void
VirtualMethodCall::ReturnValue::next() 
{
}

VirtualMethodCall::VirtualMethodCall(Smoke *smoke, Smoke::Index method, Smoke::Stack stack, QScriptValue obj, QScriptValue overridenMethod) :
    m_smoke(smoke), m_method(method), m_stack(stack), m_obj(obj),
    m_overridenMethod(overridenMethod), m_current(-1), m_called(false), 
    m_methodRef(smoke->methods[method])
{
    m_engine = m_obj.engine();
    m_args = m_smoke->argumentList + m_methodRef.args;
    
    for (int count = 0; count < m_methodRef.numArgs; count++) {
        m_valueList << m_engine->newObject();
    }
}

VirtualMethodCall::~VirtualMethodCall() {
}

void
VirtualMethodCall::unsupported() {
    m_called = true;
    
    engine()->currentContext()->throwError( QScriptContext::TypeError, 
                                            QString("Cannot handle '%1' as argument of virtual method %2::%3")
                                                    .arg(type().name())
                                                    .arg(m_smoke->className(m_methodRef.classId))
                                                    .arg(m_smoke->methodNames[m_methodRef.name]) );
}

void
VirtualMethodCall::callMethod() {
    if (m_called) {
        return;
    }
    
    m_called = true;
    QScriptValue value = m_overridenMethod.call(m_obj, m_valueList);
    ReturnValue result(m_smoke, m_method, m_stack, value);
}

void
VirtualMethodCall::next() {
    int previous = m_current;
    m_current++;
    
    while (!m_called && m_current < m_methodRef.numArgs) {
        Marshall::HandlerFn fn = getMarshallFn(type());
        (*fn)(this);
        m_current++;
    }
 
    callMethod();
    m_current = previous;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
