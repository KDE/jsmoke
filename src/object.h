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

#ifndef JSMOKE_OBJECT_H
#define JSMOKE_OBJECT_H

#include <smoke.h>
#include <jsmoke_export.h>

#include <QScriptClass>
#include <QScriptEngine>

namespace JSmoke {
    
    //!When the user constructs an object, they get a scriptvalue of one of these.
    class JSMOKE_EXPORT Object : public QScriptClass
    {
    public:
        Object(QScriptEngine*);
        ~Object(); 
        
        class Instance {
        public:
            Instance() : ownership(QScriptEngine::QtOwnership) { }
            virtual void finalize();
            void dispose();
            virtual ~Instance();
            
            inline const char * className()
            {
                return classId.smoke->classes[classId.index].className;
            }

            inline void * cast(const Smoke::ModuleIndex targetId)
            {
                return classId.smoke->cast(value, classId, targetId);
            }

            static bool isSmokeObject(const QScriptValue& object);
            static Instance *get(const QScriptValue& object);
            static void set(QScriptValue& object, Instance* instance);

        public:
            void* value;
            QScriptEngine::ValueOwnership ownership;
            Smoke::ModuleIndex classId;
        };

        typedef void (*TypeResolver)(Instance *);
        
        QScriptValue::PropertyFlags propertyFlags (const QScriptValue& object, const QScriptString& name, uint id);
        QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id);
        QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id);
        QString name() const;
    };

    class JSMOKE_EXPORT SmokeQObject : public Object 
    {
    public:
        SmokeQObject( QScriptEngine* );
        ~SmokeQObject(); 
        
        class Instance : public Object::Instance {
        public:
            Instance() : Object::Instance() { }
            ~Instance() { }
        public:
            QScriptValue qobject;
        };
        
        QScriptValue::PropertyFlags propertyFlags ( const QScriptValue & object, const QScriptString & name, uint id );
        QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id);
        QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id);
        QString name() const;
    };

}

Q_DECLARE_METATYPE( JSmoke::Object::Instance* )
Q_DECLARE_METATYPE( JSmoke::SmokeQObject::Instance* )

#endif // SAMPLEIMPL_H
