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

#ifndef QTSCRIPT_GLOBAL_H
#define QTSCRIPT_GLOBAL_H

#include <QtScript/QScriptValue>
#include "metaobject.h"
#include "QtScriptSmokeBinding.h"

namespace QtScriptSmoke {
    namespace Debug {
        
        extern Q_DECL_EXPORT uint DoDebug;

        enum QtDebugChannel {
            None = 0x00,
            MethodMatches = 0x01,
            Properties = 0x02,
            Calls = 0x04,
            GC = 0x08,
            Virtual = 0x10,
            Verbose = 0x20
        };

    }

    namespace Global {
        extern Q_DECL_EXPORT QHash<Smoke*, Module> modules;
                
        extern QtScriptSmoke::Object * Object;
        extern QtScriptSmoke::SmokeQObject * SmokeQObject;
        
        extern Smoke::ModuleIndex QObjectClassId;
        extern Smoke::ModuleIndex QDateClassId;
        extern Smoke::ModuleIndex QDateTimeClassId;
        extern Smoke::ModuleIndex QTimeClassId;
        
        extern QScriptValue QtEnum;
        
        extern Q_DECL_EXPORT QScriptValue * getScriptValue(void * ptr);
        
        extern Q_DECL_EXPORT void unmapPointer( Object::Instance * instance, 
                                                Smoke::Index classId, 
                                                void * lastptr );
                                                
        extern Q_DECL_EXPORT void mapPointer(   QScriptValue * obj, 
                                                Object::Instance * instance, 
                                                Smoke::Index classId, 
                                                void * lastptr );
                                                
        extern Q_DECL_EXPORT QScriptValue wrapInstance(QScriptEngine * engine, Smoke::ModuleIndex classId, void * ptr);

    }
}

#endif