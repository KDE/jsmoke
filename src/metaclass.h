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

#ifndef JSMOKE_METACLASS_H
#define JSMOKE_METACLASS_H

#include <QScriptClass>
#include <QScriptEngine>
#include <QScriptContext>

#include <smoke.h>

#include "object.h"

namespace JSmoke {
   
    //!The QScriptValue of QWidget, QListView etc. So it's used for construction and to access class methods.
    class MetaClass : public QScriptClass
    {
    public:
        MetaClass( QScriptEngine*, const QByteArray& className, Object* object );
        ~MetaClass();
        QScriptValue prototype() const;
        QScriptValue::PropertyFlags propertyFlags (const QScriptValue & object, const QScriptString& name, uint id);
        QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint * id);
        QScriptValue property (const QScriptValue& object, const QScriptString& name, uint id);
        QVariant extension(QScriptClass::Extension extension, const QVariant& argument);
        bool supportsExtension(QScriptClass::Extension extension) const;
        QString name() const;
        Smoke::ModuleIndex classId() { return m_classId; };
        Object* object() { return m_object; };
        
    private:
        QByteArray m_className;
        Smoke::ModuleIndex m_classId;
        Object* m_object;
        QScriptValue m_proto;
    };

}

Q_DECLARE_METATYPE( QScriptContext* )
Q_DECLARE_METATYPE( QScriptValueList )

#endif // JSMOKE_METACLASS_H