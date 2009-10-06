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

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptValue>
#include <QStringList>
#include <QWidget>
#include <QtCore/QPair>

namespace QtScriptSmoke {


Object::Object( QScriptEngine* engine )
    : QScriptClass( engine )
{   }


Object::~Object()
{ }

QScriptClass::QueryFlags
Object::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    qDebug() << "[Object] queryProperty" << name << flags << id;
    if( name.toString() == "toString" )
        return 0;
    return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;

}

QScriptValue
Object::property(const QScriptValue& object, const QScriptString& name, uint id)
{
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