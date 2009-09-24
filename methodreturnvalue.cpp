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

#include "methodreturnvalue.h"

namespace QtScript {

MethodReturnValue::MethodReturnValue(Smoke *smoke, Smoke::Index method, Smoke::Stack stack) :
    m_smoke(smoke), m_method(method), m_stack(stack)
{
    Marshall::HandlerFn fn = getMarshallFn(type());
    (*fn)(this);
}

void
MethodReturnValue::unsupported()
{
    qFatal("Cannot handle '%s' as return-type of %s::%s",
           type().name(),
           strcmp(m_smoke->className(method().classId), "QGlobalSpace") == 0 ? "" : m_smoke->className(method().classId),
           m_smoke->methodNames[method().name] );
}

void
MethodReturnValue::next() {}

}
