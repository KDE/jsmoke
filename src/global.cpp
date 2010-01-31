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

static uint qHash(const Smoke::ModuleIndex& mi) {
    return qHash(mi.index) ^ qHash(mi.smoke);
}

namespace JSmoke {
    namespace Debug {
        uint DoDebug = 0;
    }
    
    namespace Global {

        
static JSmoke::Object * Object = 0;
static JSmoke::SmokeQObject * SmokeQObject = 0;

Smoke::ModuleIndex QObjectClassId;
Smoke::ModuleIndex QDateClassId;
Smoke::ModuleIndex QDateTimeClassId;
Smoke::ModuleIndex QTimeClassId;
Smoke::ModuleIndex QEventClassId;
Smoke::ModuleIndex QGraphicsItemClassId;

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
            qWarning("JSmoke::Global::getScriptValue %p -> nil", ptr);
            if (!qscriptValues()) {
                qWarning("JSmoke::Global::getScriptValue qscriptValues deleted");
            }
        }
        return 0;
    } else {
        if (Debug::DoDebug & Debug::GC) {
            qWarning("JSmoke::Global::getScriptValue %p -> %p", ptr, (void *) qscriptValues()->operator[](ptr));
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
                qWarning("JSmoke::Global::unmapPointer (%s*)%p -> %p size: %d", className, ptr, obj, qscriptValues()->size() - 1);
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
            qWarning("JSmoke::Global::mapPointer (%s*)%p -> %p size: %d", className, ptr, (void*)obj, qscriptValues()->size() + 1);
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
                qWarning("Found an invalid script value: %p", iter.value());
                Object::Instance * instance = Object::Instance::get(*(iter.value()));
               
                if (instance->value != 0) {
                    if ((Debug::DoDebug & Debug::GC) != 0) {
                        qWarning(   "%p->~%s()", 
                                    instance->value, 
                                    instance->classId.smoke->className(instance->classId.index) );
                    }
                    
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

QScriptClass* 
scriptClassFromId(QScriptEngine * engine, const Smoke::ModuleIndex& classId)
{
    QString className(classId.smoke->classes[classId.index].className); 
    QStringList components = className.split("::");
    QScriptValue classValue = engine->globalObject().property(components[0]);
        
    for (int component = 1; component < components.length(); ++component) {
        classValue = classValue.property(components[component]);
    }
    
    return classValue.scriptClass();
}

QScriptValue 
wrapInstance(QScriptEngine * engine, Smoke::ModuleIndex classId, void * ptr, QScriptEngine::ValueOwnership ownership)
{
    Object::Instance * instance = 0;
    bool isQObject = Smoke::isDerivedFrom(classId, Global::QObjectClassId);

    if (isQObject) {
        instance = new SmokeQObject::Instance();
        QObject * qobject = static_cast<QObject*>(classId.smoke->cast(ptr, classId, Global::QObjectClassId));
        static_cast<SmokeQObject::Instance*>(instance)->qobject = engine->newQObject(qobject);
    } else {
        instance = new Object::Instance();
    }

    instance->classId = classId;
    instance->value = ptr;
    instance->ownership = ownership;
    resolveType(instance);
    
    QScriptValue obj = engine->newObject(isQObject ? Global::SmokeQObject : Global::Object); 
    Object::Instance::set(obj, instance);
    
    QScriptClass* scriptClass = scriptClassFromId(engine, classId);
    obj.setProperty("prototype", scriptClass->prototype());
    
    if (ownership != QScriptEngine::QtOwnership) {
        mapPointer(new QScriptValue(obj), instance, instance->classId.index, 0);
    }
    
    return obj;
}

static QList<QPair<Smoke::ModuleIndex, Object::TypeResolver> > typeResolvers;
static QHash<Smoke::ModuleIndex, Object::TypeResolver> typeResolverMap;

void 
registerTypeResolver(const Smoke::ModuleIndex& baseClass, Object::TypeResolver typeResolver)
{
    typeResolvers << QPair<Smoke::ModuleIndex, Object::TypeResolver>(baseClass, typeResolver);
}

void 
resolveType(Object::Instance * instance)
{
    if (typeResolverMap.contains(instance->classId)) {
        Object::TypeResolver resolver = typeResolverMap[instance->classId];
        Smoke::ModuleIndex classId = instance->classId;
        (*resolver)(instance);
        instance->value = instance->classId.smoke->cast(instance->value, classId, instance->classId);
    }
}

void
initializeClasses(QScriptEngine * engine, Smoke * smoke)
{
    if (Global::Object == 0) {
        // This is wrong because it won't work with multiple engines, as there is
        // only one Global::Object for all engines
        Global::Object = new JSmoke::Object(engine);
        Global::SmokeQObject = new JSmoke::SmokeQObject(engine);
    }
    
    for (int i = 1; i <= smoke->numClasses; i++) {
        Smoke::ModuleIndex classId(smoke, i);
        QScriptClass * klass = 0;        
        QByteArray className(smoke->classes[i].className);        

        if (smoke->classes[i].external || className == "QGlobalSpace") {
            continue;
        }
        
        for (int i = 0; i < typeResolvers.size(); ++i) {
            if (Smoke::isDerivedFrom(classId, typeResolvers.at(i).first)) {
                typeResolverMap[classId] = typeResolvers.at(i).second;
            }
        }
        
        if (Smoke::isDerivedFrom(classId, Global::QObjectClassId)) {
            klass = new JSmoke::MetaObject(engine, className, Global::SmokeQObject);
        } else {
            klass = new JSmoke::MetaObject(engine, className, Global::Object);
        }
        
        if (className.contains("::")) {
            QStringList components = QString(className).split("::");
            QScriptValue outerClass = engine->globalObject().property(components[0]);
            
            if (!outerClass.isValid()) {
                outerClass = engine->newObject(new JSmoke::MetaObject(  engine, 
                                                                        components[0].toLatin1(), 
                                                                        Global::Object ) );
                engine->globalObject().setProperty(components[0], outerClass);
            }
            
            for (int component = 1; component < components.length() - 1; ++component) {
                QScriptValue temp = outerClass.property(components[component]);
                
                if (!temp.isValid()) {
                    temp = engine->newObject(new JSmoke::MetaObject(    engine, 
                                                                        components[component].toLatin1(), 
                                                                        Global::Object ) );
                    outerClass.setProperty(components[component], temp);
                }
                
                outerClass = temp;
            }
            
            outerClass.setProperty(components.last(), engine->newObject(klass));
        } else {
            engine->globalObject().setProperty(QString(className), engine->newObject(klass));
        }
    }
}

    } // namespace Global
} // namespace JSmoke
