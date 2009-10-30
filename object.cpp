/*
 * Copyright 2009 Ian Monroe <imonroe@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "object.h"

#include "SmokeQtScriptUtils.h"
#include "metaobject.h"
#include "methodcall.h"
#include "global.h"

#include <smoke/qtcore_smoke.h>

#include <QtDebug>

#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptValue>

#include <QStringList>
#include <QObject>
#include <QMetaMethod>
#include <QMetaObject>
#include <QWidget>
#include <QtCore/QPair>

namespace QtScriptSmoke {


Object::Object( QScriptEngine* engine )
    : QScriptClass( engine )
{   }


Object::~Object()
{ }

void Object::Instance::finalize() 
{
    switch (ownership) {
    case QScriptEngine::QtOwnership:
        break;
    case QScriptEngine::ScriptOwnership:
        if (value != 0) {
            dispose();
        }
        break;
    case QScriptEngine::AutoOwnership:
//        if (value != 0 && value->parent() == 0) {
//            dispose();
//        }
        break;
    }
}

void Object::Instance::dispose()
{
    const char *className = classId.smoke->classes[classId.index].className;

    if  ((Debug::DoDebug & Debug::GC) != 0) {
        qWarning("Deleting (%s*)%p", className, value);
    }
    
    QByteArray methodName(className);
    methodName.prepend("~");
    Smoke::ModuleIndex nameId = classId.smoke->findMethodName(className, methodName);
    Smoke::ModuleIndex methodId = classId.smoke->findMethod(classId, nameId);
    
    if(methodId.index > 0) {
        Smoke::Method &methodRef = methodId.smoke->methods[methodId.smoke->methodMaps[methodId.index].method];
        Smoke::ClassFn fn = methodId.smoke->classes[methodRef.classId].classFn;
        Smoke::StackItem destroyInstanceStack[1];
        (*fn)(methodRef.method, value, destroyInstanceStack);
    }
    
    value = 0;
}

Object::Instance::~Instance() {
    finalize();
}

bool Object::Instance::isSmokeObject(const QScriptValue &object)
{
    return  object.data().isVariant() 
            && object.data().toVariant().canConvert<Object::Instance*>();
}

Object::Instance * Object::Instance::get(const QScriptValue &object) 
{
    return object.data().toVariant().value<Object::Instance*>();
}

void Object::Instance::set(QScriptValue &object, Object::Instance * instance)
{
    object.setData(object.engine()->newVariant(QVariant::fromValue<Object::Instance*>(instance)));        
}

QScriptValue::PropertyFlags 
Object::propertyFlags ( const QScriptValue & object, const QScriptString & name, uint id )
{
    // qDebug() << "Object::propertyFlags(" << name << "," << id << ")";
    return QScriptValue::ReadOnly;
}

QScriptClass::QueryFlags
Object::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    if ((Debug::DoDebug & Debug::Properties) != 0) {
        Object::Instance * instance = Object::Instance::get(object);
        qWarning("Object::queryProperty(%p->%s.%s, 0x%2.2x, %d)", 
                 instance->value,
                 instance->classId.smoke->classes[instance->classId.index].className,
                 name.toString().toLatin1().constData(), 
                 (uint) flags, 
                 *id);
    }
    
    if( name.toString() == QLatin1String("valueOf") )
        return 0;

    // We don't handle write access because a script can override a virtual
    // method by overwriting a property with a custom function.
    return QScriptClass::HandlesReadAccess;
}

QScriptValue
Object::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    if ((Debug::DoDebug & Debug::Properties) != 0) {
        Object::Instance * instance = Object::Instance::get(object);
        qWarning("Object::property(%p->%s.%s, %d)", 
                 instance->value,
                 instance->classId.smoke->classes[instance->classId.index].className,
                 name.toString().toLatin1().constData(), 
                 id);
    }
    
    if (name.toString() == QLatin1String("toString")) {
        Object::Instance * instance = Object::Instance::get(object);
        Smoke::ModuleIndex methodId = instance->classId.smoke->findMethod(  instance->classId.smoke->classes[instance->classId.index].className, 
                                                                            "toString" );
        if (methodId.index == 0) {
            return engine()->newFunction(instanceToString);
        }
    }
    
    QString nameStr = name;
    QScriptValue fn = engine()->newFunction( callSmokeMethod );
    fn.setData( QScriptValue( name ) );
    return fn;
}

QString
Object::name() const
{
    return "QtScriptSmoke::Object";
}

SmokeQObject::SmokeQObject(QScriptEngine* engine)
    : Object(engine) { }


SmokeQObject::~SmokeQObject() { }

QScriptValue::PropertyFlags 
SmokeQObject::propertyFlags(const QScriptValue& object, const QScriptString & name, uint id)
{
    SmokeQObject::Instance * instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(object));
    QByteArray propertyName(name.toString().toLatin1());

    if (propertyName == "metaObject") {
        return QScriptValue::ReadOnly;
    }
    
    if ((Debug::DoDebug & Debug::Properties) != 0) {
        qWarning("SmokeQObject::propertyFlags(%p->%s.%s, %d)", 
                instance->value,
                instance->classId.smoke->classes[instance->classId.index].className,
                name.toString().toLatin1().constData(), 
                id);
    }

    QObject * qobject = static_cast<QObject*>(instance->classId.smoke->cast(    instance->value, 
                                                                                instance->classId.index, 
                                                                                Global::QObjectClassId.index ) );
                                                                               
    const QMetaObject * meta = qobject->metaObject();
    while (meta != 0) {
        for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
            if (propertyName == meta->property(i).name()) {
                return 0;
            }
        }
        
        meta = meta->superClass();
    }
    
    return Object::propertyFlags(object, name, id);
}

QScriptClass::QueryFlags
SmokeQObject::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    SmokeQObject::Instance * instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(object));

    if (name.toString() == QLatin1String("metaObject")) {
        return QScriptClass::HandlesReadAccess;
    }

    if ((Debug::DoDebug & Debug::Properties) != 0) {
        qWarning("SmokeQObject::queryProperty(%p->%s.%s, 0x%2.2x, %d)", 
                 instance->value,
                 instance->classId.smoke->classes[instance->classId.index].className,
                 name.toString().toLatin1().constData(), 
                 (uint) flags, 
                 *id);
    }

    if (    (flags & QScriptClass::HandlesWriteAccess) != 0 
            && propertyFlags(object, name, *id) == 0 ) 
    {
#ifdef WE_UNDERSTAND_H9W_THE_HELL_TO_GET_PROPERTY_SETTERS_WORKING
        return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
#else
        return QScriptClass::HandlesReadAccess;
#endif
    }

    return Object::queryProperty(object, name, flags, id);
}

QScriptValue
SmokeQObject::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    SmokeQObject::Instance * instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(object));
    QByteArray propertyName(name.toString().toLatin1());

    if ((Debug::DoDebug & Debug::Properties) != 0) {
        Object::Instance * instance = Object::Instance::get(object);
        qWarning("SmokeQObject::property(%p->%s.%s, %d)", 
                 instance->value,
                 instance->classId.smoke->classes[instance->classId.index].className,
                 propertyName.constData(), 
                 id);
    }
    
    QObject * qobject = static_cast<QObject*>(instance->classId.smoke->cast(    instance->value, 
                                                                                instance->classId.index, 
                                                                                Global::QObjectClassId.index ) );
    const QMetaObject * meta = qobject->metaObject();
    
    while (meta != 0) {
        for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
            if (meta->method(i).methodType() == QMetaMethod::Signal) {
                QByteArray signalName(meta->method(i).signature());
                signalName = signalName.mid(0, signalName.indexOf('('));
                
                if (signalName == propertyName) {
                    // qDebug() << "Matched a property with signal name" << signalName;
                    /*
                        FIXME: This nearly works. but If a string is used as a target it doesn't work
                        timer.timeout.connect(this, "update()");   // Doesn't work
                        timer.timeout.connect(this, this.update);  // Works
                    */
                    return instance->qobject.property(name);
                }
            }
        }
        
        meta = meta->superClass();
    }
    
    meta = qobject->metaObject();
    while (meta != 0) {
        for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
            if (propertyName == meta->property(i).name()) {
                QScriptValue thisObject = object.engine()->currentContext()->thisObject();
                SmokeQObject::Instance * instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(thisObject));
                if (instance != 0) {
                    qobject = static_cast<QObject*>(instance->classId.smoke->cast(  instance->value, 
                                                                                    instance->classId.index, 
                                                                                    Global::QObjectClassId.index ) );
                }
                
                return valueFromVariant(engine(), qobject->property(propertyName));
            }
        }
        
        meta = meta->superClass();
    }

    return Object::property(object, name, id);
}

void 
SmokeQObject::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    QScriptValue thisObject = object.engine()->currentContext()->thisObject();
    SmokeQObject::Instance * instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(thisObject));
    if (instance == 0) {
        instance = static_cast<SmokeQObject::Instance*>(Object::Instance::get(object));
    }
    
    QByteArray propertyName(name.toString().toLatin1());

    if ((Debug::DoDebug & Debug::Properties) != 0) {
        qWarning("SmokeQObject::setProperty(%p->%s.%s, %d, %s)", 
                 instance->value,
                 instance->classId.smoke->classes[instance->classId.index].className,
                 propertyName.constData(), 
                 id,
                 value.toString().toLatin1().constData() );
    }

    QObject * qobject = static_cast<QObject*>(instance->classId.smoke->cast(    instance->value, 
                                                                                instance->classId.index, 
                                                                                Global::QObjectClassId.index ) );
    qobject->setProperty(propertyName, valueToVariant(value));
    return;
}

QString
SmokeQObject::name() const
{
    return "QtScriptSmoke::SmokeQObject";
}

}
