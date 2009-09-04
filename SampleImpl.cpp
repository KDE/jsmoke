/*
    QtScript-Smoke: Plays around with the idea of using smoke for qtscript
    Copyright (C) 2009 Ian Monroe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "SampleImpl.h"

#include "SampleClass.h"

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptValue>

SampleImpl::SampleImpl( QScriptEngine* engine )
    : QScriptClass( engine )
{   }


SampleImpl::~SampleImpl()
{ }

QScriptClass::QueryFlags
SampleImpl::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    qDebug() << "[SampleImpl] queryProperty" << name << flags << id;
    return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;

}

QScriptValue
SampleImpl::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    qDebug() << "[SampleImpl] property" << name << id;
    qDebug() << "is variant?" << object.data().isVariant();
    qDebug() << "is attrobj?" << object.data().toVariant().canConvert<AttributedObject*>();
    QString nameStr = name;
    AttributedObject* attrObj = object.data().toVariant().value<AttributedObject*>();
    if( nameStr == "show" )
    {
        Smoke::ModuleIndex classId = qt_Smoke->findClass( attrObj->className );
        Smoke::Class klass = classId.smoke->classes[classId.index];
        Smoke::ModuleIndex methId = qt_Smoke->findMethod( attrObj->className, nameStr.toLocal8Bit());
        Smoke::Method meth = methId.smoke->methods[methId.smoke->methodMaps[methId.index].method];
        qDebug() << "class name" << attrObj->className << "method name" << nameStr;
        (*klass.classFn)(meth.method, attrObj->object, 0);
    }
    return engine()->newObject();
}

QString
SampleImpl::name() const
{
    return "SampleImpl";
}

