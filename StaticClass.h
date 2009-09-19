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

#ifndef SAMPLECLASS_H
#define SAMPLECLASS_H

#include <QScriptClass>
#include <smoke.h>

//so that one instance of ImplementationClass can serve all objects
//maybe this isn't worth it though
struct AttributedObject
{
    QByteArray className;
    void* object;
};
Q_DECLARE_METATYPE( AttributedObject* )

class ImplementationClass;

//I'm open to a better name :)
//!The QScriptValue of QWidget, QListView etc. So it's used for construction and to access class methods.
class StaticClass : public QScriptClass
{
    public:
        StaticClass( QScriptEngine*, const QByteArray& className, ImplementationClass* implClass );
        ~StaticClass();
        QScriptValue prototype() const;
        QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id );
        QScriptValue property ( const QScriptValue & object, const QScriptString & name, uint id );
        QVariant extension( QScriptClass::Extension extension, const QVariant& argument );
        bool supportsExtension( QScriptClass::Extension extension ) const;
    private:
        QByteArray m_className;
        ImplementationClass* m_implClass;
};

#include <QScriptContext>
Q_DECLARE_METATYPE( QScriptContext* )

#endif