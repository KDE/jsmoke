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

#include "metaobject.h"

#include "object.h"
#include "smokebinding.h"
#include "methodcall.h"

#include <smoke/qtcore_smoke.h>

#include <QtDebug>
#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptString>
#include <QVariant>

#include "utils.h"
#include "global.h"

namespace QtScriptSmoke {
    
    
// This will be called for any call() invocation, not just something like 
// QWidget.call(this, parent); which is what the code below is trying to do.
//
// The aim is to push a new activation and change the 'thisObject' to be the first
// argument of the call. Then create a new arguments object without that first arg,
// and call the 'QWidget' constructor. Whether it is successfully doing that isn't
// obvious...
static QScriptValue 
callFunctionInvocation(QScriptContext* context, QScriptEngine* engine)
{
    MetaObject * metaObject = static_cast<MetaObject*>(context->thisObject().scriptClass());
    Smoke::ModuleIndex classId = metaObject->classId();
    
    QScriptContext * constructorContext = engine->pushContext();
    constructorContext->setThisObject(context->argument(0));

    QScriptValue args = context->argumentsObject();
    args.property("shift").call(args);
    constructorContext->activationObject().setProperty("arguments", args);
    
    QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(    classId, 
                                                                                        classId.smoke->classes[classId.index].className,                                                                                        constructorContext );
    if (matches.count() == 0) {
        QString message = QString("overloaded %1() constructor not resolved").arg(classId.smoke->classes[classId.index].className);
        return context->throwError(QScriptContext::TypeError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        QString message = QString("overloaded %1() constructor not resolved").arg(classId.smoke->classes[classId.index].className);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }

    QtScriptSmoke::MethodCall methodCall(classId.smoke, matches[0].first.index, constructorContext, constructorContext->engine());
    methodCall.next();
    engine->popContext();
    return engine->undefinedValue();
}

MetaObject::MetaObject( QScriptEngine* engine, const QByteArray& className, Object * object )
    : QScriptClass( engine )
    , m_className( className )
    , m_classId( qtcore_Smoke->findClass(className.constData()) )
    , m_object( object )
{ 
    m_proto = engine->newObject();
}

MetaObject::~MetaObject()
{  }

QScriptValue
MetaObject::prototype() const
{
    return m_proto;
}

QScriptValue::PropertyFlags 
MetaObject::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    // qDebug() << "MetaObject::propertyFlags(" << m_className << name << "," << id << ")";
    return  QScriptValue::Undeletable 
            | QScriptValue::SkipInEnumeration;
//            | QScriptValue::ReadOnly;
}

QScriptClass::QueryFlags
MetaObject::queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id)
{
    QByteArray propertyName(name.toString().toLatin1());
    
    if ((Debug::DoDebug & Debug::Properties) != 0) {
        qWarning("MetaObject::queryProperty(%s.%s, 0x%2.2x, %d)", 
                 m_className.constData(),
                 propertyName.constData(), 
                 (uint) flags, 
                 *id);
    }

    // qDebug() << "MetaObject::queryProperty(" << name << "," << flags << "," << *id << ")";
    
    if (propertyName == "prototype") {
        return flags & (QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess);
    } else if ( m_className == "Qt" 
                && (    propertyName == "Debug"
                        || propertyName == "Enum" ) ) 
    {
        return 0;
    } else if (qtcore_Smoke->findClass(m_className + "::" + propertyName) != qtcore_Smoke->NullModuleIndex) {
        return 0;
    } else {
        return QScriptClass::HandlesReadAccess; 
    }
}

QScriptValue
MetaObject::property(const QScriptValue& object, const QScriptString & name, uint id)
{
    QByteArray propertyName(name.toString().toLatin1());
    
    if ((Debug::DoDebug & Debug::Properties) != 0) {
        qWarning("MetaObject::property(%s.%s, %d)", 
                 m_className.constData(),
                 propertyName.constData(), 
                 id);
    }

    if (propertyName == "prototype") {
        return m_proto;
    } else if (propertyName == "toString") {
        return QScriptValue(engine(), QString(m_className).replace("::", "."));
    } else if (propertyName == "call") {
        return engine()->newFunction(callFunctionInvocation);
    } else {
        // Look for enums and if found, return the value directly
        Smoke::Class & klass = m_classId.smoke->classes[m_classId.index];
        Smoke::ModuleIndex methodId = m_classId.smoke->findMethod(klass.className, propertyName);
        if (methodId.index != 0) {
            Smoke::Index ix = methodId.smoke->methodMaps[methodId.index].method;
            if (ix > 0 && (m_classId.smoke->methods[ix].flags & Smoke::mf_enum) != 0) {
                QtScriptSmoke::MethodCall methodCall(m_classId.smoke, ix, object.engine()->currentContext(), object.engine());
                methodCall.next();
                return *(methodCall.var());
            }
        }
    }

    QScriptValue fn = engine()->newFunction(callSmokeStaticMethod);
    fn.setData(QScriptValue(name));
    return fn;
}

QVariant
MetaObject::extension(QScriptClass::Extension extension, const QVariant& argument)
{
    if (extension == Callable) {
        QScriptContext *context = qvariant_cast<QScriptContext*>(argument);
        QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(m_classId, m_className.constData(), context);

        if (matches.count() == 0) {
            QString message = QString("overloaded %1() constructor not resolved").arg(m_className.constData());
            context->setThisObject(context->throwError(QScriptContext::TypeError, message));
            return QVariant();
        } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
            QString message = QString("overloaded %1() constructor not resolved").arg(m_className.constData());
            context->setThisObject(context->throwError(QScriptContext::TypeError, message));
            return QVariant();
        } else {
            // Good, found a single best match in matches[0]
        }
        
        context->thisObject().setScriptClass(object());
        context->thisObject().setProperty("prototype", m_proto);

        QtScriptSmoke::MethodCall methodCall(m_classId.smoke, matches[0].first.index, context, context->engine());
        methodCall.next();
        
        // Set the 'this object' in case an exception has been thrown in the method call
        context->setThisObject(*(methodCall.var()));
        return QVariant();
    } else if (extension == HasInstance) {
        QScriptValueList args = argument.value<QScriptValueList>();  
        MetaObject * scriptClass = static_cast<MetaObject*>(args[0].scriptClass());
        Object::Instance * instance = Object::Instance::get(args[1]);
        bool result = qtcore_Smoke->isDerivedFrom(instance->classId, scriptClass->m_classId);
        return QVariant(result);
    }
}

bool
MetaObject::supportsExtension(QScriptClass::Extension extension) const
{
    return extension == Callable || extension == HasInstance;
}

QString
MetaObject::name() const
{
    return m_className;
}

}