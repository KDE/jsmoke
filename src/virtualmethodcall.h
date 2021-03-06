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

#ifndef JSMOKE_VIRTUAL_METHOD_CALL_H
#define JSMOKE_VIRTUAL_METHOD_CALL_H

#include <smoke.h>
#include "marshall.h"

namespace JSmoke {

    class VirtualMethodCall : public Marshall {
        
        class ReturnValue : Marshall {
        public:
            ReturnValue(Smoke::ModuleIndex methodId, Smoke::Stack stack, QScriptValue returnValue);

            inline const Smoke::Method &method() { return m_methodId.smoke->methods[m_methodId.index]; }
            inline SmokeType type() { return m_type; }
            inline Marshall::Action action() { return Marshall::FromQScriptValue; }
            inline Smoke::StackItem &item() { return m_stack[0]; }
            inline QScriptEngine * engine() { return 0; }
            inline QScriptValue * var() { return &m_returnValue; }
            inline Smoke *smoke() { return m_methodId.smoke; }
            inline bool cleanup() { return false; }

            void unsupported();
            void next();
            
        private:
            Smoke::ModuleIndex m_methodId;
            Smoke::Stack m_stack;
            SmokeType m_type;
            QScriptValue m_returnValue;
        };
        
    public:
        VirtualMethodCall(Smoke::ModuleIndex methodId, Smoke::Stack stack, QScriptValue obj, QScriptValue overridenMethod);

        ~VirtualMethodCall();

        inline SmokeType type() { return SmokeType(m_methodId.smoke, m_args[m_current]); }
        inline Marshall::Action action() { return Marshall::ToQScriptValue; }
        inline Smoke::StackItem &item() { return m_stack[m_current + 1]; }
        inline QScriptEngine * engine() { return m_engine; }
        inline QScriptValue * var() { return &(m_valueList[m_current]); }
        inline const Smoke::Method &method() { return m_methodRef; }
        inline Smoke *smoke() { return m_methodId.smoke; }
        inline bool cleanup() { return false; }   // is this right?

        void unsupported();
        void callMethod();
        void next();
        
    private:
        Smoke::ModuleIndex m_methodId;
        Smoke::Stack m_stack;
        QScriptEngine * m_engine;
        QScriptValue m_obj;
        QScriptValue m_overridenMethod;
        int m_current;
        Smoke::Index * m_args;
        QScriptValueList m_valueList;
        bool m_called;
        Smoke::Method & m_methodRef;
    };

}

#endif // JSMOKE_VIRTUAL_METHOD_CALL_H

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;

