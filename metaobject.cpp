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
#include "QtScriptSmokeBinding.h"
#include "methodcall.h"

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptString>
#include <QVariant>

#include "SmokeQtScriptUtils.h"
#include "global.h"

namespace QtScriptSmoke {
    
void Instance::finalize(QScriptEngine *engine) 
{
    switch (ownership) {
    case QScriptEngine::QtOwnership:
        break;
    case QScriptEngine::ScriptOwnership:
//        if (value)
//            engine->disposeQObject(value);
        break;
    case QScriptEngine::AutoOwnership:
//        if (value && !value->parent())
//            engine->disposeQObject(value);
        break;
    }
}

bool Instance::isSmokeObject(const QScriptValue &object)
{
    return  object.data().isVariant() 
            && object.data().toVariant().canConvert<QtScriptSmoke::Instance*>();
}

Instance * Instance::get(const QScriptValue &object) 
{
    return object.data().toVariant().value<QtScriptSmoke::Instance*>();
}

void Instance::set(QScriptValue &object, Instance * instance)
{
    object.setData(object.engine()->newVariant(QVariant::fromValue<QtScriptSmoke::Instance*>(instance)));        
}

MetaObject::MetaObject( QScriptEngine* engine, const QByteArray& className, Object * object )
    : QScriptClass( engine )
    , m_className( className )
    , m_classId( qt_Smoke->findClass(className.constData()) )
    , m_object( object )
{ }

MetaObject::~MetaObject()
{  }

QScriptValue
MetaObject::prototype() const
{
    //this fn is called, but i'm pretty sure this function doesn't 
    //do anything, due to the Callable extension
    return QScriptValue();
}

QScriptValue::PropertyFlags 
MetaObject::propertyFlags ( const QScriptValue & object, const QScriptString & name, uint id )
{
    qDebug() << "MetaObject::propertyFlags(" << name << "," << id << ")";
    return QScriptValue::ReadOnly;
}

QScriptClass::QueryFlags
MetaObject::queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id )
{
    //qDebug() << object.toVariant();
    qDebug() << "MetaObject::queryProperty(" << name << "," << flags << "," << *id << ")";
    
    if (    name.toString() == "prototype"
            || name.toString() == "toString"
            || name.toString() == "valueOf" )
    {
        return 0;
    } else {
        return QScriptClass::HandlesReadAccess; 
    }
}

// This will be called for any call() invocation, not just something like 
// QWidget.call(this, parent); which is what the code below is trying to do.
//
// The aim is to push a new activation and change the 'thisObject' to be the first
// argument of the call. Then create a new arguments object without that first arg,
// and call the 'QWidget' constructor. Whether it is successfully doing that isn't
// obvious...
QScriptValue 
callFunctionInvocation(QScriptContext* context, QScriptEngine* engine)
{
    QScriptClass * cls = context->thisObject().scriptClass();
    Smoke::ModuleIndex classId = static_cast<MetaObject*>(cls)->classId();
    
    QScriptContext * constructorContext = engine->pushContext();
    constructorContext->setThisObject(context->argument(0));
    
    QScriptValue args = context->argumentsObject();
    args.property("shift").call(args);
    constructorContext->activationObject().setProperty("arguments", args);
    
    QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(    classId, 
                                                                                        classId.smoke->classes[classId.index].className, 
                                                                                        constructorContext );
    if (matches.count() == 0) {
        QString message = QString("overloaded %1() constructor not resolved").arg(classId.smoke->classes[classId.index].className);
        return context->throwError(QScriptContext::TypeError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        QString message = QString("overloaded %1() constructor not resolved").arg(classId.smoke->classes[classId.index].className);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }

    QtScriptSmoke::MethodCall methodCall(qt_Smoke, matches[0].first.index, constructorContext, constructorContext->engine());
    methodCall.next();
    engine->popContext();
    
    return *(methodCall.var());
}

QScriptValue
MetaObject::property ( const QScriptValue & object, const QScriptString & name, uint id )
{
    qDebug() << "MetaObject::property(" << name << "," << id << ")";

    if( name == engine()->toStringHandle("prototype") )
    {
        qDebug() << "its asking for the prototype";
        //return m_proto;
        return engine()->newObject();
    } else if (name.toString() == "call") {
        return engine()->newFunction(callFunctionInvocation);
    } else {
        // Look for enums and if found, return the value directly
        Smoke::Class & klass = m_classId.smoke->classes[m_classId.index];
        Smoke::ModuleIndex methodId = m_classId.smoke->findMethod(klass.className, name.toString().toLatin1().constData());
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
MetaObject::extension( QScriptClass::Extension extension, const QVariant& argument )
{
    if( extension == Callable )
    {
        //I tried the following ninjutsu, but the functionName is blank
        //QScriptContextInfo cInfo( context );
        //QByteArray classNameByteArray = cInfo.functionName().toLatin1();
        //const char* className = classNameByteArray.constData();
        //qDebug() << "we find the classname to be" << className

        QScriptContext* context = argument.value<QScriptContext*>();
        for (int count = 0; count < context->argumentCount(); count++) {
            printf("arg: %s\n", context->argument(count).toString().toLatin1().constData());
        }
        
        // printf("context callee: %s\n", context->callee().toString().toLatin1().constData());
        qDebug() << "constructor?" << context->isCalledAsConstructor() << context->backtrace();
        
        QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(m_classId, m_className.constData(), context);

        if (matches.count() == 0) {
            QString message = QString("overloaded %1() constructor not resolved").arg(m_className.constData());
            context->setThisObject(context->throwError(QScriptContext::TypeError, message));
            return 15;
        } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
            QString message = QString("overloaded %1() constructor not resolved").arg(m_className.constData());
            context->setThisObject(context->throwError(QScriptContext::TypeError, message));
            return 15;
        } else {
            // Good, found a single best match in matches[0]
        }
        
        QScriptValue proto = context->engine()->newObject(m_object); 
        context->setThisObject(proto);
        QtScriptSmoke::MethodCall methodCall(qt_Smoke, matches[0].first.index, context, context->engine());
        methodCall.next();
        context->setThisObject(*(methodCall.var()));
        return 15;
    }
}

bool
MetaObject::supportsExtension( QScriptClass::Extension extension ) const
{
        if( extension == Callable )
            return true;
}

}