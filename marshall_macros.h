/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

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

#ifndef QTSCRIPT_SMOKE_MARSHALL_MACROS_H
#define QTSCRIPT_SMOKE_MARSHALL_MACROS_H

/*
    These functions are based on the ones in qscriptengine.h, but adapted to use Smoke instances
*/


inline QScriptValue qScriptSmokeValueFromSequence_helper(QScriptEngine *eng, int id, void * ptr)
{
    QScriptValue * value = QtScriptSmoke::Global::getScriptValue(ptr);
    if (value != 0) {
        return *value;
    }
    
    const char * typeName = QMetaType::typeName(id);
    printf("qScriptSmokeValueFromSequence() arg typeName: %s\n", typeName);
    Smoke::ModuleIndex classId = qt_Smoke->idClass(typeName);
    QtScriptSmoke::Instance * instance = new QtScriptSmoke::Instance();
    instance->classId = classId;
    instance->value = ptr;
    instance->ownership = QScriptEngine::QtOwnership;
    QScriptValue obj = eng->newObject(QtScriptSmoke::Global::Object); 
    QtScriptSmoke::Instance::set(obj, instance);
    return obj;
}

template <class Container>
QScriptValue qScriptSmokeValueFromSequence(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    quint32 i;
    for (it = begin, i = 0; it != end; ++it, ++i) {
        int id = qMetaTypeId<typename Container::value_type>();
        a.setProperty(i, qScriptSmokeValueFromSequence_helper(eng, id, (void *) &(*it)));
    }
    
    return a;
}

inline void * qScriptSmokeValueToSequence_helper(const QScriptValue& item, int id)
{
    QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(item);
    void * ptr = instance->value;
    if (id == 0) {
        printf("Error type not registered\n");
    } else {
        const char * typeName = QMetaType::typeName(id);
        printf("qScriptSmokeValueToSequence() arg typeName: %s\n", typeName);
        Smoke::ModuleIndex classId = qt_Smoke->idClass(typeName);
        
        if (instance->classId.smoke == classId.smoke) {
            ptr = instance->classId.smoke->cast(ptr, instance->classId.index, classId.index);
        } else {
            // If the method's class and the instance's class are in different smoke modules
            // then we need to convert them both to be class ids in the instance's module in
            // order to do the cast
            const Smoke::Class &klass = classId.smoke->classes[classId.index];
            ptr = instance->classId.smoke->cast(    ptr, 
                                                    instance->classId.index, 
                                                    instance->classId.smoke->idClass(klass.className, true).index );            
        }
    }
    
    return ptr;
}

template <class Container>
void qScriptSmokeValueToSequence(const QScriptValue &value, Container &cont)
{
    quint32 len = value.property(QLatin1String("length")).toUInt32();
    for (quint32 i = 0; i < len; ++i) {
        int id = qMetaTypeId<typename Container::value_type>();
                
#if defined Q_CC_MSVC && !defined Q_CC_MSVC_NET
        cont.push_back(*(static_cast<Container::value_type *>(qScriptSmokeValueToSequence_helper(value.property(i), id))));
#else
        cont.push_back(*(static_cast<typename Container::value_type *>(qScriptSmokeValueToSequence_helper(value.property(i), id))));
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

#endif