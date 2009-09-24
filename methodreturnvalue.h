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

#ifndef QTSCRIPT_METHOD_RETURN_VALUE_H
#define QTSCRIPT_METHOD_RETURN_VALUE_H

#include <smoke.h>
#include "marshall.h"

namespace QtScript {

class Q_DECL_EXPORT MethodReturnValue : public Marshall {
    
public:
    MethodReturnValue(Smoke *smoke, Smoke::Index method, Smoke::Stack stack);

    inline const Smoke::Method &method() { return m_smoke->methods[m_method]; }
    inline SmokeType type() { return SmokeType(m_smoke, method().ret); }
    inline Marshall::Action action() { return Marshall::ToQScriptValue; }
    inline Smoke::StackItem &item() { return m_stack[0]; }
    inline QScriptValue var() { return *m_returnValue; }
    inline Smoke *smoke() { return m_smoke; }
    inline bool cleanup() { return false; }

    void unsupported();
    void next();
    
private:
    Smoke *m_smoke;
    Smoke::Index m_method;
    QScriptValue * m_returnValue;
    Smoke::Stack m_stack;
};

}

#endif // QTSCRIPT_METHOD_RETURN_VALUE_H
