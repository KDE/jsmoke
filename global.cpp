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

namespace QtScriptSmoke {
    namespace Debug {
        int DoDebug = 0;
    }
    
    namespace Global {

        
QtScriptSmoke::Object * Object = 0;
QtScriptSmoke::Binding binding;

Smoke::ModuleIndex QObjectClassId;
Smoke::ModuleIndex QDateClassId;
Smoke::ModuleIndex QDateTimeClassId;
Smoke::ModuleIndex QTimeClassId;

typedef QHash<void *, QScriptValue *> QScriptValuesMap;
Q_GLOBAL_STATIC(QScriptValuesMap, qscriptValues)

QScriptValue * 
getScriptValue(void *ptr) 
{
    if (!qscriptValues() || !qscriptValues()->contains(ptr)) {
        if (Debug::DoDebug & Debug::GC) {
            qWarning("QtScriptSmoke::Global::getScriptValue %p -> nil", ptr);
            if (!qscriptValues()) {
                qWarning("QtScriptSmoke::Global::getScriptValue qscriptValues deleted");
            }
        }
        return 0;
    } else {
        if (Debug::DoDebug & Debug::GC) {
            qWarning("QtScriptSmoke::Global::getScriptValue %p -> %p", ptr, (void *) qscriptValues()->operator[](ptr));
        }
        return qscriptValues()->operator[](ptr);
    }
}

void 
unmapPointer(QtScriptSmoke::Instance * instance, Smoke::Index classId, void *lastptr) 
{
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
    if (ptr != lastptr) {
        lastptr = ptr;
        if (qscriptValues() && qscriptValues()->contains(ptr)) {
            QScriptValue * obj = qscriptValues()->operator[](ptr);
            
            if (Debug::DoDebug & Debug::GC) {
                QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(*obj);
                const char *className = instance->classId.smoke->classes[instance->classId.index].className;
                qWarning("QtScriptSmoke::Global::unmapPointer (%s*)%p -> %p size: %d", className, ptr, obj, qscriptValues()->size() - 1);
            }
            
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
mapPointer(QScriptValue * obj, QtScriptSmoke::Instance * instance, Smoke::Index classId, void *lastptr) 
{
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
     
    if (ptr != lastptr) {
        lastptr = ptr; 
        
        if (Debug::DoDebug & Debug::GC) {
            QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(*obj);
            const char *className = instance->classId.smoke->classes[instance->classId.index].className;
            qWarning("QtScriptSmoke::Global::mapPointer (%s*)%p -> %p size: %d", className, ptr, (void*)obj, qscriptValues()->size() + 1);
        }
        
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

QScriptValue 
wrapInstance(QScriptEngine * engine, Smoke::ModuleIndex classId, void * ptr)
{
    QtScriptSmoke::Instance * instance = new QtScriptSmoke::Instance();
    instance->classId = classId;
    instance->value = ptr;
    QScriptValue obj = engine->newObject(QtScriptSmoke::Global::Object); 
    QtScriptSmoke::Instance::set(obj, instance);
    return obj;
}

    }
}
