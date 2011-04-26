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
#include "debug.h"

namespace JSmoke {

    namespace Global {
        JSMOKE_EXPORT extern QHash<Smoke*, Module> modules;
        
        JSMOKE_EXPORT extern Smoke::ModuleIndex QObjectClassId;
        JSMOKE_EXPORT extern Smoke::ModuleIndex QDateClassId;
        JSMOKE_EXPORT extern Smoke::ModuleIndex QDateTimeClassId;
        JSMOKE_EXPORT extern Smoke::ModuleIndex QTimeClassId;
        JSMOKE_EXPORT extern Smoke::ModuleIndex QEventClassId;
        JSMOKE_EXPORT extern Smoke::ModuleIndex QGraphicsItemClassId;
        
        JSMOKE_EXPORT extern QScriptValue QtEnum;
        
        JSMOKE_EXPORT void startFinalizerThread();
        
        JSMOKE_EXPORT QScriptValue * getScriptValue(void * ptr);
        
        JSMOKE_EXPORT void unmapPointer(    Object::Instance * instance, 
                                            const Smoke::ModuleIndex& classId, 
                                            void * lastptr = 0 );
                                                
        JSMOKE_EXPORT void mapPointer(  QScriptValue * obj, 
                                        Object::Instance * instance, 
                                        const Smoke::ModuleIndex& classId,
                                        void * lastptr = 0 );
                                        
        JSMOKE_EXPORT QScriptClass* scriptClassFromId(QScriptEngine * engine, const Smoke::ModuleIndex& classId);
                                                                                                
        JSMOKE_EXPORT QScriptValue wrapInstance(    QScriptEngine * engine, 
                                                    const Smoke::ModuleIndex& classId, 
                                                    void * ptr,
                                                    QScriptEngine::ValueOwnership ownership = QScriptEngine::QtOwnership );

        JSMOKE_EXPORT void registerTypeResolver(const Smoke::ModuleIndex& baseClass, Object::TypeResolver);
        JSMOKE_EXPORT void resolveType(Object::Instance * instance);
        
        JSMOKE_EXPORT void initializeClasses(QScriptEngine * engine, Smoke * smoke);
    }
}

#endif