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

#include "ImplementationClass.h"

#include "SmokeQtScriptUtils.h"
#include "StaticClass.h"

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptValue>
#include <QStringList>

ImplementationClass::ImplementationClass( QScriptEngine* engine )
    : QScriptClass( engine )
{   }


ImplementationClass::~ImplementationClass()
{ }

QScriptClass::QueryFlags
ImplementationClass::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    qDebug() << "[ImplementationClass] queryProperty" << name << flags << id;
    if( name.toString() == "toString" )
        return 0;
    return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;

}

QScriptValue runSmokeMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    AttributedObject* attrObj = context->thisObject().data().toVariant().value<AttributedObject*>();
    qDebug() << "[ImplementationClass] runSmokeMethod" << nameFn << attrObj;
    Smoke::ModuleIndex classId = qt_Smoke->findClass( attrObj->className );
    Smoke::Class klass = classId.smoke->classes[classId.index];
    qDebug() << "[ImplementationClass] class loaded:" << klass.className;

    Smoke::ModuleIndex methId;
    QByteArray mungedArgs = SmokeQtScript::mungedArgs( context );
    methId = qt_Smoke->findMethod( attrObj->className, nameFn.toLatin1()  + mungedArgs );
    Smoke::Method meth = methId.smoke->methods[methId.smoke->methodMaps[methId.index].method];
    qDebug() << "[ImplementationClass] property called with argumentCount of:" << context->argumentCount() 
        <<  qt_Smoke->methodNames[meth.name] << mungedArgs;

    Smoke::StackItem args[context->argumentCount() + 1];
    SmokeQtScript::scriptArgumentsToSmoke( context, args );
    (*klass.classFn)(meth.method, attrObj->object, args);
     //TODO: return value
    return QScriptValue();
}

QScriptValue
ImplementationClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    QString nameStr = name;
    QScriptValue fn = engine()->newFunction( runSmokeMethod );
    fn.setData( QScriptValue( name ) );
    return fn;
}

QString
ImplementationClass::name() const
{
    return "ImplementationClass";
}

