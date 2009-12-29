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

#ifndef JSMOKE_GLOBAL_H
#define JSMOKE_GLOBAL_H

#include <QtCore/QHash>
#include <QtScript/QScriptValue>

#include <smoke.h>

#include "jsmoke_export.h"
#include "metaobject.h"
#include "smokebinding.h"

inline uint qHash(const Smoke::ModuleIndex& mi) {
    return qHash(mi.index) ^ qHash(mi.smoke);
}
        
namespace JSmoke {
    namespace Debug {
        
        extern JSMOKE_EXPORT uint DoDebug;

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
        extern JSMOKE_EXPORT QHash<Smoke*, Module> modules;
        
        extern JSMOKE_EXPORT Smoke::ModuleIndex QObjectClassId;
        extern JSMOKE_EXPORT Smoke::ModuleIndex QDateClassId;
        extern JSMOKE_EXPORT Smoke::ModuleIndex QDateTimeClassId;
        extern JSMOKE_EXPORT Smoke::ModuleIndex QTimeClassId;
        extern JSMOKE_EXPORT Smoke::ModuleIndex QEventClassId;
        extern JSMOKE_EXPORT Smoke::ModuleIndex QGraphicsItemClassId;
        
        extern JSMOKE_EXPORT QScriptValue QtEnum;
        
        extern JSMOKE_EXPORT void startFinalizerThread();
        
        extern JSMOKE_EXPORT QScriptValue * getScriptValue(void * ptr);
        
        extern JSMOKE_EXPORT void unmapPointer( Object::Instance * instance, 
                                                Smoke::Index classId, 
                                                void * lastptr );
                                                
        extern JSMOKE_EXPORT void mapPointer(   QScriptValue * obj, 
                                                Object::Instance * instance, 
                                                Smoke::Index classId, 
                                                void * lastptr );
                                                                                                
        extern JSMOKE_EXPORT QScriptValue wrapInstance( QScriptEngine * engine, 
                                                        Smoke::ModuleIndex classId, 
                                                        void * ptr,
                                                        QScriptEngine::ValueOwnership ownership = QScriptEngine::QtOwnership );

        extern JSMOKE_EXPORT void registerTypeResolver(const Smoke::ModuleIndex& baseClass, Object::TypeResolver);
        extern JSMOKE_EXPORT void resolveType(Object::Instance * instance);
        
        extern JSMOKE_EXPORT void initializeClasses(QScriptEngine * engine, Smoke * smoke);
    }
}

#endif