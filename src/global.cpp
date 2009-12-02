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

#include <stdlib.h>

#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

#include "global.h"
#include "smoke/qtcore_smoke.h"

namespace QtScriptSmoke {
    namespace Debug {
        uint DoDebug = 0;
    }
    
    namespace Global {

        
static QtScriptSmoke::Object * Object = 0;
static QtScriptSmoke::SmokeQObject * SmokeQObject = 0;

Smoke::ModuleIndex QObjectClassId;
Smoke::ModuleIndex QDateClassId;
Smoke::ModuleIndex QDateTimeClassId;
Smoke::ModuleIndex QTimeClassId;

QScriptValue QtEnum;

QHash<Smoke*, Module> modules;

typedef QHash<void *, QScriptValue *> QScriptValuesMap;
Q_GLOBAL_STATIC(QScriptValuesMap, qscriptValues)

static QMutex mutex(QMutex::Recursive);

QScriptValue * 
getScriptValue(void *ptr) 
{
    QMutexLocker locker(&mutex);

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
unmapPointer(Object::Instance * instance, Smoke::Index classId, void *lastptr) 
{
    QMutexLocker locker(&mutex);
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
    
    if (ptr != lastptr) {
        lastptr = ptr;
        if (qscriptValues() && qscriptValues()->contains(ptr)) {
            QScriptValue * obj = qscriptValues()->operator[](ptr);
            
            if (Debug::DoDebug & Debug::GC) {
                Object::Instance * instance = Object::Instance::get(*obj);
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
mapPointer(QScriptValue * obj, Object::Instance * instance, Smoke::Index classId, void *lastptr) 
{
    QMutexLocker locker(&mutex);
    Smoke * smoke = instance->classId.smoke;
    void * ptr = smoke->cast(instance->value, instance->classId.index, classId);
     
    if (ptr != lastptr) {
        lastptr = ptr; 
        
        if (Debug::DoDebug & Debug::GC) {
            Object::Instance * instance = Object::Instance::get(*obj);
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



class FinalizerThread : public QThread
{
public:
    static const int ScriptValuesSweepInterval = 5;
    
    void run()
    {
        while (true) {
            setTerminationEnabled(true);
            QThread::sleep(ScriptValuesSweepInterval);                        
            sweepScriptValues();
        }
    }
    
    void sweepScriptValues()
    {
        QMutexLocker locker(&mutex);    
        QMutableHashIterator<void *, QScriptValue *> iter(*(qscriptValues()));
        
        while (iter.hasNext()) {
            iter.next();
            
            if (!iter.value()->isValid()) {
                setTerminationEnabled(false);               
                qWarning("Found an invalid script value: %s", iter.value()->toString().toLatin1().constData());
                Object::Instance * instance = Object::Instance::get(*(iter.value()));

                if ((Debug::DoDebug & Debug::GC) != 0) {
                    qWarning(   "%p->~%s()", 
                                instance->value, 
                                instance->classId.smoke->className(instance->classId.index) );
                }
                
                if (instance->value != 0) {
                    unmapPointer(instance, instance->classId.index, 0);
                    instance->finalize();
                }
                
                delete iter.value();
                setTerminationEnabled(true);
           }
        }
    }

};

static FinalizerThread finalizer;

void
terminateFinalizerThread()
{
    finalizer.terminate();
    finalizer.wait();
    return;
}

void
startFinalizerThread()
{
    atexit(terminateFinalizerThread);
    finalizer.start();
    return;
}

QScriptValue 
wrapInstance(QScriptEngine * engine, Smoke::ModuleIndex classId, void * ptr, QScriptEngine::ValueOwnership ownership)
{
    Object::Instance * instance = 0;
    bool isQObject = qtcore_Smoke->isDerivedFrom(   classId.smoke, 
                                                    classId.index,
                                                    Global::QObjectClassId.smoke,
                                                    Global::QObjectClassId.index );

    if (isQObject) {
        instance = new SmokeQObject::Instance();
        QObject * obj = static_cast<QObject*>(classId.smoke->cast(  ptr, 
                                                                    classId.index, 
                                                                    Global::QObjectClassId.index ) );
        (static_cast<SmokeQObject::Instance*>(instance))->qobject = engine->newQObject(obj);
    } else {
        instance = new Object::Instance();
    }

    instance->classId = classId;
    instance->value = ptr;
    instance->ownership = ownership;
    QScriptValue obj = engine->newObject(isQObject ? Global::SmokeQObject : Global::Object); 
    Object::Instance::set(obj, instance);
    
    if (ownership != QScriptEngine::QtOwnership) {
        mapPointer(new QScriptValue(obj), instance, instance->classId.index, 0);
    }
    
    return obj;
}

void
initializeClasses(QScriptEngine * engine, Smoke * smoke)
{
    if (Global::Object == 0) {
        Global::Object = new QtScriptSmoke::Object(engine);
        Global::SmokeQObject = new QtScriptSmoke::SmokeQObject(engine);
    }
    
    for (int i = 1; i <= smoke->numClasses; i++) {
        QByteArray className(smoke->classes[i].className);        
        QScriptClass * klass = 0;
        
        if (smoke->isDerivedFrom(   smoke, 
                                    i,
                                    Global::QObjectClassId.smoke,
                                    Global::QObjectClassId.index ) )
        {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    className, 
                                                    Global::SmokeQObject );
        } else {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    className, 
                                                    Global::Object );
        }
        
        if (className.contains("::")) {
            QStringList components = QString(className).split("::");
            QScriptValue outerClass = engine->globalObject().property(components[0]);
            
            for (int component = 1; component < components.length() - 1; ++component) {
                outerClass = outerClass.property(components[component]);
            }
            
            outerClass.setProperty(components.last(), engine->newObject(klass));
        } else {
            engine->globalObject().setProperty(QString(className), engine->newObject(klass));
        }
    }
}


    } // namespace Global
} // namespace QtScriptSmoke
