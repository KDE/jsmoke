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

#include <smoke/qt_smoke.h>

#include <QtDebug>
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
    // qDebug() << "Object::queryProperty(" << name << "," << flags << "," << *id << ")";
    
    if( name.toString() == QLatin1String("toString") )
        return 0;
    
    if( name.toString() == QLatin1String("valueOf") )
        return 0;

    // We don't handle write access because a script can override a virtual
    // method by overwriting a property with a custom function.
    return QScriptClass::HandlesReadAccess;
}

QScriptValue
Object::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    // qDebug() << "Object::property(" << name << "," << id << ")";
    
    if (Object::Instance::isSmokeObject(object)) {
        Object::Instance * instance = Object::Instance::get(object);
        if (qt_Smoke->isDerivedFrom(    instance->classId.smoke, 
                                        instance->classId.index,
                                        Global::QObjectClassId.smoke,
                                        Global::QObjectClassId.index ) )
        {
            QObject * qobject = static_cast<QObject*>(instance->classId.smoke->cast(    instance->value, 
                                                                                        instance->classId.index, 
                                                                                        Global::QObjectClassId.index ) );
            const QMetaObject * meta = qobject->metaObject();
            
            for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
                if (meta->method(i).methodType() == QMetaMethod::Signal) {
                    QByteArray signalName(meta->method(i).signature());
                    signalName = signalName.mid(0, signalName.indexOf('('));
                    
                    if (signalName == name.toString().toLatin1()) {
                        qDebug() << "Matched a property with signal name" << signalName;
                        // FIXME: Sadly, this doesn't actually work - there are no error messages, but a
                        // target slot connected to this signal is never called. Maybe it is being GC'd,
                        // but putting a copy of it into a static variable didn't help.
                        QScriptValue signalFunction = object.engine()->newQObject(qobject).property(name);
                        return signalFunction;
                    }
                }
            }
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

}