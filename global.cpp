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

#include "global.h"
#include <QtCore/QHash>

namespace QtScript {
    namespace Debug {
        int DoDebug = 0;
    }
    
    namespace Global {

QtScriptSmokeBinding binding;

typedef QHash<void *, QScriptValue *> QScriptValuesMap;
Q_GLOBAL_STATIC(QScriptValuesMap, qscriptValues)

QScriptValue * 
getScriptValue(void *ptr) 
{
    if (!qscriptValues() || !qscriptValues()->contains(ptr)) {
        return 0;
    } else {
        return qscriptValues()->operator[](ptr);
    }
}

void 
unmapPointer(QtScript::SmokeInstance * instance, Smoke::Index classId, void *lastptr) 
{
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
    if (ptr != lastptr) {
        lastptr = ptr;
        if (qscriptValues() && qscriptValues()->contains(ptr)) {
            QScriptValue * obj = qscriptValues()->operator[](ptr);        
            qscriptValues()->remove(ptr);
        }
    }

    for (   Smoke::Index * parent = smoke->inheritanceList + smoke->classes[classId].parents; 
            *parent != 0; 
            parent++ ) 
    {
        unmapPointer(instance, *parent, lastptr);
    }
}

// Store pointer in g_qscriptValues hash : "pointer_to_Qt_object" => weak ref to associated QScriptValue object
// Recurse to store it also as casted to its parent classes.

void 
mapPointer(QScriptValue * obj, QtScript::SmokeInstance * instance, Smoke::Index classId, void *lastptr) 
{
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
     
    if (ptr != lastptr) {
        lastptr = ptr;     
        qscriptValues()->insert(ptr, obj);
    }
    
    for (   Smoke::Index * parent = smoke->inheritanceList + smoke->classes[classId].parents; 
            *parent != 0; 
            parent++ ) 
    {
        mapPointer(obj, instance, *parent, lastptr);
    }
    
    return;
}

    }
}
