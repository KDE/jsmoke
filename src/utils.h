/*
 * Copyright 2009 Ian Monroe <imonroe@kde.org>
 * Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>
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

#ifndef JSMOKE_UTILS_H
#define JSMOKE_UTILS_H

#include <smoke.h>

#include <QtCore/QVector>
#include <QtCore/QByteArray>
#include <QtCore/QPair>

#include "jsmoke_export.h"
#include "object.h"

class QScriptContext;

namespace JSmoke {
    JSMOKE_EXPORT QString methodToString(Smoke::ModuleIndex methodId);
    JSMOKE_EXPORT QVector<QByteArray> mungedMethods( const QByteArray& nameFn, QScriptContext* context );
    JSMOKE_EXPORT QVector<QPair<Smoke::ModuleIndex, int> > resolveMethod(Smoke::ModuleIndex classId, const QByteArray& methodName, QScriptContext* context);
    JSMOKE_EXPORT QScriptValue callSmokeStaticMethod(QScriptContext* context, QScriptEngine* engine);
    JSMOKE_EXPORT QScriptValue callSmokeMethod(QScriptContext* context, QScriptEngine* engine);
    JSMOKE_EXPORT QScriptValue instanceToString(QScriptContext* context, QScriptEngine* engine);
    JSMOKE_EXPORT void * constructCopy(Object::Instance *instance);
    JSMOKE_EXPORT QVariant valueToVariant(const QScriptValue& value);
    JSMOKE_EXPORT QScriptValue valueFromVariant(QScriptEngine *engine, const QVariant& variant);
}

#endif