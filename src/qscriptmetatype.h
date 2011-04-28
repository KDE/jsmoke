/*
 *   Copyright 2009-2010 by Richard Dale <richard.j.dale@gmail.com>

 *   Adapted from the code in src/script/qscriptengine.h in Qt 4.5
 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef JSMOKE_QSCRIPTMETATYPE_H
#define JSMOKE_QSCRIPTMETATYPE_H

#include <QtScript/QScriptValueIterator>

#include "marshall.h"
#include "object.h"
#include "utils.h"

/*
    Allows a metatype to be declared for a type containing a single comma.
    For example:
        Q_DECLARE_METATYPE2(QList<QPair<QByteArray,QByteArray> >)       
 */
#define Q_DECLARE_METATYPE2(TYPE1, TYPE2)                               \
    QT_BEGIN_NAMESPACE                                                  \
    template <>                                                         \
    struct QMetaTypeId< TYPE1,TYPE2 >                                   \
    {                                                                   \
        enum { Defined = 1 };                                           \
        static int qt_metatype_id()                                     \
            {                                                           \
                static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
                if (!metatype_id)                                       \
                    metatype_id = qRegisterMetaType< TYPE1,TYPE2 >( #TYPE1 "," #TYPE2, \
                               reinterpret_cast< TYPE1,TYPE2  *>(quintptr(-1))); \
                return metatype_id;                                     \
            }                                                           \
    };                                                                  \
    QT_END_NAMESPACE

namespace JSmoke {
    
template <class Container>
void marshall_Container(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {        
        m->item().s_voidp = new Container(qscriptvalue_cast<Container>(*(m->var())));        
        m->next();
        
        if (!m->type().isConst()) {
            *(m->var()) = m->engine()->toScriptValue(*(static_cast<Container*>(m->item().s_voidp)));
        }
        
        if (m->cleanup()) {
            delete static_cast<Container*>(m->item().s_voidp);
        }        
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        *(m->var()) = m->engine()->toScriptValue(*(static_cast<Container*>(m->item().s_voidp)));        
        m->next();
        
        if (!m->type().isConst()) {
            *(static_cast<Container*>(m->item().s_voidp)) = qscriptvalue_cast<Container>(*(m->var()));
        }
        
        if (m->cleanup()) {
            delete static_cast<Container*>(m->item().s_voidp);
        }
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

}

/*
    These functions are based on the ones in qscriptengine.h, but adapted to use Smoke instances
*/


inline QScriptValue qScriptSmokeValueFromSequence_helper(QScriptEngine *eng, Smoke::ModuleIndex classId, void * ptr)
{
    QScriptValue * value = JSmoke::Global::getScriptValue(ptr);
    if (value != 0) {
        return *value;
    }
    
    return JSmoke::Global::wrapInstance(eng, classId, ptr);
}

template <class Container>
QScriptValue qScriptSmokeValueFromSequence(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    const char * typeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type>());
    Smoke::ModuleIndex classId = Smoke::findClass(typeName);
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    quint32 i;
    for (it = begin, i = 0; it != end; ++it, ++i) {
        a.setProperty(i, qScriptSmokeValueFromSequence_helper(eng, classId, (void *) &(*it)));
    }
    
    return a;
}

inline void * qScriptSmokeValueToSequence_helper(const QScriptValue& item, Smoke::ModuleIndex classId)
{
    JSmoke::Object::Instance * instance = JSmoke::Object::Instance::get(item);
    return instance->cast(classId);
}

template <class Container>
void qScriptSmokeValueToSequence(const QScriptValue &value, Container &cont)
{
    quint32 len = value.property(QLatin1String("length")).toUInt32();
    const char * typeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type>());
    Smoke::ModuleIndex classId = Smoke::findClass(typeName);
    for (quint32 i = 0; i < len; ++i) {
                
#if defined Q_CC_MSVC && !defined Q_CC_MSVC_NET
        cont.push_back(*(static_cast<Container::value_type *>(qScriptSmokeValueToSequence_helper(value.property(i), classId))));
#else
        cont.push_back(*(static_cast<typename Container::value_type *>(qScriptSmokeValueToSequence_helper(value.property(i), classId))));
#endif
    }
}

template<typename T>
int qScriptSmokeRegisterSequenceMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , T * /* dummy */ = 0
#endif
)
{
    return qScriptRegisterMetaType<T>(engine, qScriptSmokeValueFromSequence,
                                      qScriptSmokeValueToSequence, prototype);
}

template <class Container>
QScriptValue qScriptSmokeValueFromPointerSequence(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    QByteArray typeName(QMetaType::typeName(qMetaTypeId<typename Container::value_type>()));
    // Remove the star from the type name
    typeName.chop(1);
    Smoke::ModuleIndex classId = Smoke::findClass(typeName);
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    quint32 i;
    for (it = begin, i = 0; it != end; ++it, ++i) {
        a.setProperty(i, qScriptSmokeValueFromSequence_helper(eng, classId, (void *) *it));
    }
    
    return a;
}

template <class Container>
void qScriptSmokeValueToPointerSequence(const QScriptValue &value, Container &cont)
{
    quint32 len = value.property(QLatin1String("length")).toUInt32();
    QByteArray typeName(QMetaType::typeName(qMetaTypeId<typename Container::value_type>()));
    // Remove the star from the type name
    typeName.chop(1);
    Smoke::ModuleIndex classId = Smoke::findClass(typeName);
    for (quint32 i = 0; i < len; ++i) {
                
#if defined Q_CC_MSVC && !defined Q_CC_MSVC_NET
        cont.push_back(static_cast<Container::value_type>(qScriptSmokeValueToSequence_helper(value.property(i), classId)));
#else
        cont.push_back(static_cast<typename Container::value_type>(qScriptSmokeValueToSequence_helper(value.property(i), classId)));
#endif
    }
}

template<typename T>
int qScriptSmokeRegisterPointerSequenceMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , T * /* dummy */ = 0
#endif
)
{
    return qScriptRegisterMetaType<T>(engine, qScriptSmokeValueFromPointerSequence,
                                      qScriptSmokeValueToPointerSequence, prototype);
}


template <class Container>
QScriptValue qScriptSmokeValueFromPairSequence(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    const char * firstTypeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type::first_type>());
    const char * secondTypeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type::second_type>());
    Smoke::ModuleIndex firstClassId = Smoke::findClass(firstTypeName);
    Smoke::ModuleIndex secondClassId = Smoke::findClass(secondTypeName);
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    quint32 i;
    for (it = begin, i = 0; it != end; ++it, ++i) {
        QScriptValue pair = eng->newArray();
        if (firstClassId == Smoke::NullModuleIndex) {
            pair.setProperty(0, eng->toScriptValue((*it).first));
        } else {
            pair.setProperty(0, qScriptSmokeValueFromSequence_helper(eng, firstClassId, (void *) &((*it).first)));
        }
        
        if (secondClassId == Smoke::NullModuleIndex) {
            pair.setProperty(1, eng->toScriptValue((*it).second));
        } else {
            pair.setProperty(1, qScriptSmokeValueFromSequence_helper(eng, secondClassId, (void *) &((*it).second)));
        }
        
        a.setProperty(i, pair);
    }
    
    return a;
}

template <class Container>
void qScriptSmokeValueToPairSequence(const QScriptValue &value, Container &container)
{
    quint32 len = value.property(QLatin1String("length")).toUInt32();
    const char * firstTypeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type::first_type>());
    const char * secondTypeName = QMetaType::typeName(qMetaTypeId<typename Container::value_type::second_type>());
    Smoke::ModuleIndex firstClassId = Smoke::findClass(firstTypeName);
    Smoke::ModuleIndex secondClassId = Smoke::findClass(secondTypeName);
    for (quint32 i = 0; i < len; ++i) {
        QScriptValue pair = value.property(i);
        
        if (firstClassId == Smoke::NullModuleIndex) {
            if (secondClassId == Smoke::NullModuleIndex) {
                container.push_back(QPair<typename Container::value_type::first_type, typename Container::value_type::second_type>(
                    qscriptvalue_cast<typename Container::value_type::first_type>(pair.property(0)),
                    qscriptvalue_cast<typename Container::value_type::second_type>(pair.property(1)) ) );
            } else {
                container.push_back(QPair<typename Container::value_type::first_type, typename Container::value_type::second_type>(
                    qscriptvalue_cast<typename Container::value_type::first_type>(pair.property(0)),
                    *(static_cast<typename Container::value_type::second_type *>(qScriptSmokeValueToSequence_helper(pair.property(1), secondClassId))) ) );
            }
        } else {
            if (secondClassId == Smoke::NullModuleIndex) {
                container.push_back(QPair<typename Container::value_type::first_type, typename Container::value_type::second_type>(
                    *(static_cast<typename Container::value_type::first_type *>(qScriptSmokeValueToSequence_helper(pair.property(0), firstClassId))),
                    qscriptvalue_cast<typename Container::value_type::second_type>(pair.property(1)) ) );
            } else {
                container.push_back(QPair<typename Container::value_type::first_type, typename Container::value_type::second_type>(
                    *(static_cast<typename Container::value_type::first_type *>(qScriptSmokeValueToSequence_helper(pair.property(0), firstClassId))),
                    *(static_cast<typename Container::value_type::second_type *>(qScriptSmokeValueToSequence_helper(pair.property(1), secondClassId))) ) );
            }
        }
    }
}

template<typename T>
int qScriptSmokeRegisterPairSequenceMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , T * /* dummy */ = 0
#endif
)
{
    return qScriptRegisterMetaType<T>(engine, qScriptSmokeValueFromPairSequence,
                                      qScriptSmokeValueToPairSequence, prototype);
}

template <class Container>
QScriptValue qScriptSmokeValueFromHash(QScriptEngine *eng, const Container &container)
{
    QScriptValue value = eng->newObject();
    const char * keyTypeName = QMetaType::typeName(qMetaTypeId<typename Container::key_type>());
    const char * mappedTypeName = QMetaType::typeName(qMetaTypeId<typename Container::mapped_type>());
    Smoke::ModuleIndex keyClassId = Smoke::findClass(keyTypeName);
    Smoke::ModuleIndex mappedClassId = Smoke::findClass(mappedTypeName);
    typename Container::const_iterator begin = container.begin();
    typename Container::const_iterator end = container.end();
    typename Container::const_iterator it;
    for (it = begin; it != end; ++it) {
        QScriptValue key;
        
        if (keyClassId == Smoke::NullModuleIndex) {
            key = eng->toScriptValue(it.key());
        } else {
            key = qScriptSmokeValueFromSequence_helper(eng, keyClassId, (void *) &(it.key()));
        }
        
        if (mappedClassId == Smoke::NullModuleIndex) {
            value.setProperty(key.toString(), eng->toScriptValue(it.value()));
        } else {
            value.setProperty(key.toString(), qScriptSmokeValueFromSequence_helper(eng, mappedClassId, (void *) &(it.value())));
        }
    }
    
    return value;
}

template <class Container>
void qScriptSmokeValueToHash(const QScriptValue &value, Container &container)
{
    const char * mappedTypeName = QMetaType::typeName(qMetaTypeId<typename Container::mapped_type>());
    Smoke::ModuleIndex mappedClassId = Smoke::findClass(mappedTypeName);    
    QScriptValueIterator it(value);
    
    while (it.hasNext()) {
        it.next();
        
        if (mappedClassId == Smoke::NullModuleIndex) {
            container[qscriptvalue_cast<typename Container::key_type>(it.name())] = 
                qscriptvalue_cast<typename Container::mapped_type>(it.value());
        } else {
            container[qscriptvalue_cast<typename Container::key_type>(it.name())] = 
                *(static_cast<typename Container::mapped_type *>(qScriptSmokeValueToSequence_helper(it.value(), mappedClassId)));
        }
    }
}

template<typename T>
int qScriptSmokeRegisterHashMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , T * /* dummy */ = 0
#endif
)
{
    return qScriptRegisterMetaType<T>(engine, qScriptSmokeValueFromHash,
                                      qScriptSmokeValueToHash, prototype);
}
#endif // JSMOKE_QSCRIPTMETATYPE_H
