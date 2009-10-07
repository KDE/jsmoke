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

QScriptClass::QueryFlags
MetaObject::queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id )
{
    //qDebug() << object.toVariant();
    qDebug() << "MetaObject::queryProperty" << name << flags << id;
    if( engine()->toStringHandle("prototype") == name )
    {
      //  return QScriptClass::HandlesReadAccess;
        return 0;
    }
    else
        return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
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
    QtScriptSmoke::MethodCall methodCall(qt_Smoke, matches[0].first.index, constructorContext, constructorContext->engine());
    methodCall.next();
    engine->popContext();
    
    return *(methodCall.var());
}

QScriptValue
MetaObject::property ( const QScriptValue & object, const QScriptString & name, uint id )
{
    qDebug() << "MetaObject::property" << name << id;

    if( name == engine()->toStringHandle("prototype") )
    {
        qDebug() << "its asking for the prototype";
        //return m_proto;
        return engine()->newObject();
    } else if (name == engine()->toStringHandle("call")) {
        return engine()->newFunction(callFunctionInvocation);
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
            // Error
        } else if (matches.count() > 1) {
            // Error
        } else {
            // Good, found a single match in matches[0]
        }
        
        QScriptValue proto = context->engine()->newObject(m_object); 
        context->setThisObject(proto);
        QtScriptSmoke::MethodCall methodCall(qt_Smoke, matches[0].first.index, context, context->engine());
        methodCall.next();
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