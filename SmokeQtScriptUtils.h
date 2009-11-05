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

#include <smoke.h>

#include <QtCore/QVector>
#include <QtCore/QByteArray>
#include <QtCore/QPair>

#include "object.h"

class QScriptContext;

namespace QtScriptSmoke
{
    QString methodToString(Smoke::ModuleIndex methodId);
    QVector<QByteArray> mungedMethods( const QByteArray& nameFn, QScriptContext* context );
    QVector<QPair<Smoke::ModuleIndex, int> > resolveMethod(Smoke::ModuleIndex classId, const QByteArray& methodName, QScriptContext* context);
    QScriptValue callSmokeStaticMethod(QScriptContext* context, QScriptEngine* engine);
    QScriptValue callSmokeMethod(QScriptContext* context, QScriptEngine* engine);
    QScriptValue instanceToString(QScriptContext* context, QScriptEngine* engine);
    void * constructCopy(Object::Instance *instance);
    QVariant valueToVariant(const QScriptValue& value);
    QScriptValue valueFromVariant(QScriptEngine *engine, const QVariant& variant);
    QScriptValue QVariant_valueOf(QScriptContext* context, QScriptEngine* engine);
    QScriptValue QVariant_fromValue(QScriptContext* context, QScriptEngine* engine);
}