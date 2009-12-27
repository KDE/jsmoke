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

#ifndef JSMOKE_METHOD_CALL_H
#define JSMOKE_METHOD_CALL_H

#include <smoke.h>

#include "jsmoke_export.h"
#include "marshall.h"
#include "metaobject.h"


namespace JSmoke {

    class JSMOKE_EXPORT MethodCall : public Marshall {
        
        class ReturnValue : Marshall {
            
        public:
            ReturnValue(Smoke *smoke, Smoke::Index method, Smoke::Stack stack, QScriptEngine * engine, QScriptValue * returnValue);

            inline const Smoke::Method &method() { return m_smoke->methods[m_method]; }
            inline SmokeType type() { return SmokeType(m_smoke, method().ret); }
            inline Marshall::Action action() { return Marshall::ToQScriptValue; }
            inline Smoke::StackItem &item() { return m_stack[0]; }
            inline QScriptEngine * engine() { return m_engine; }
            inline QScriptValue * var() { return m_returnValue; }
            inline Smoke *smoke() { return m_smoke; }
            inline bool cleanup() { return false; }

            void unsupported();
            void next();
            
        private:
            Smoke *m_smoke;
            Smoke::Index m_method;
            Smoke::Stack m_stack;
            QScriptEngine * m_engine;
            QScriptValue * m_returnValue;
        };
        
    public:
        MethodCall(Smoke * smoke, Smoke::Index method, QScriptContext * context, QScriptEngine * engine);
        ~MethodCall();

        inline SmokeType type() { return SmokeType(m_smoke, m_args[m_current]); }
        inline Marshall::Action action() { return Marshall::FromQScriptValue; }
        inline Smoke::StackItem &item() { return m_stack[m_current + 1]; }
        inline QScriptEngine * engine() { return m_engine; }
        inline QScriptValue * var() {
            if (m_current < 0 || m_error) {
                return &m_returnValue;
            }
            
            return &(m_valueList[m_current]);
        }
        inline Smoke *smoke() { return m_smoke; }
        inline bool cleanup() { return true; }

        void unsupported();

        void callMethod();
        void next();
            
    private:
        int m_current;
        Smoke * m_smoke;
        Smoke::Stack m_stack;
        Smoke::Index m_method;
        Smoke::Index * m_args;
        QScriptContext * m_context;
        QScriptEngine * m_engine;
        QScriptValue m_target;
        JSmoke::Object::Instance * m_instance;
        QScriptValue m_returnValue;
        QScriptValueList m_valueList;
        bool m_called;
        bool m_error;
        Smoke::Method & m_methodRef;
    };

}

#endif // JSMOKE_METHOD_CALL_H

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;

