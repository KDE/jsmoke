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

#include "virtualmethodreturnvalue.h"

namespace QtScriptSmoke {

VirtualMethodReturnValue::VirtualMethodReturnValue(Smoke *smoke, Smoke::Index meth, Smoke::Stack stack, QScriptValue returnValue) :
    m_smoke(smoke), m_method(meth), m_stack(stack), m_returnValue(returnValue) 
{
    m_type.set(m_smoke, method().ret);
    Marshall::HandlerFn fn = getMarshallFn(type());
    (*fn)(this);
}

void
VirtualMethodReturnValue::unsupported()
{
    qFatal("Cannot handle '%s' as return-type of virtual method %s::%s",
            type().name(),
            m_smoke->className(method().classId),
            m_smoke->methodNames[method().name] );
}

void
VirtualMethodReturnValue::next() {}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
