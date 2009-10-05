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

#ifndef STATIC_CLASS_H
#define STATIC_CLASS_H

#include <QScriptClass>
#include <QScriptEngine>
#include <QScriptContext>

#include <smoke.h>

#include "object.h"

namespace QtScriptSmoke {
   
class Instance {
public:
    Instance() : ownership(QScriptEngine::QtOwnership) { }
    virtual void finalize(QScriptEngine *engine);
    virtual ~Instance() {}

    static bool isSmokeObject(const QScriptValue &object);
    static Instance *get(const QScriptValue &object);
    static void set(QScriptValue &object, Instance * instance);

public:
    void * value;
    QScriptEngine::ValueOwnership ownership;
    Smoke::ModuleIndex classId;
};

//!The QScriptValue of QWidget, QListView etc. So it's used for construction and to access class methods.
class MetaObject : public QScriptClass
{
    public:
        MetaObject( QScriptEngine*, const QByteArray& className, Object * implClass );
        ~MetaObject();
        QScriptValue prototype() const;
        QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id );
        QScriptValue property ( const QScriptValue & object, const QScriptString & name, uint id );
        QVariant extension( QScriptClass::Extension extension, const QVariant& argument );
        bool supportsExtension( QScriptClass::Extension extension ) const;
    private:
        QByteArray m_className;
        Object* m_implClass;
};

}

Q_DECLARE_METATYPE( QScriptContext* )
Q_DECLARE_METATYPE( QtScriptSmoke::Instance* )

#endif