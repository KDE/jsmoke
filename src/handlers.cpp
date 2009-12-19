/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   Based on the PerlQt marshalling code by Ashley Winters

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

#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QLocale>
#include <QtCore/QRegExp>

#include "marshall.h"
#include "global.h"
#include "utils.h"

Q_DECLARE_METATYPE(QLocale::Country)
Q_DECLARE_METATYPE(QList<QLocale::Country>)

namespace JSmoke {
    
static void marshall_basetype(Marshall *m)
{
    switch(m->type().element()) {        
    case Smoke::t_bool:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (!value.isBool()) {
                m->item().s_bool = false;
            } else {
                m->item().s_bool = value.toBool();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_bool);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
    
    case Smoke::t_char:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_char = 0;
            } else {
                m->item().s_char = (char) value.toInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_char);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
    
    case Smoke::t_uchar:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_uchar = 0;
            } else {
                m->item().s_uchar = (uchar) value.toUInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_uchar);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
    
    case Smoke::t_short:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_short = 0;
            } else {
                m->item().s_short = (short) value.toInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_short);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_ushort:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_ushort = 0;
            } else {
                m->item().s_ushort = value.toUInt16();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_ushort);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_int:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_int = 0;
            } else {
                m->item().s_int = value.toInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_int);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_uint:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_uint = 0;
            } else {
                m->item().s_uint = value.toUInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_uint);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_long:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_long = 0;
            } else {
                m->item().s_long = (long) value.toInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), (int) m->item().s_long);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_ulong:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_ulong = 0;
            } else {
                m->item().s_ulong = (ulong) value.toUInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), (uint) m->item().s_ulong);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_float:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_float = 0.0;
            } else {
                m->item().s_float = (float) value.toNumber();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_float);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_double:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_double = 0.0;
            } else {
                m->item().s_double = value.toNumber();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), m->item().s_double);
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_enum:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_enum = 0;
            } else if (value.instanceOf(JSmoke::Global::QtEnum)) {
                m->item().s_enum = value.property("value").toUInt32();
            } else {
                m->item().s_enum = value.toUInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
        {
            QScriptValueList args;
            args << (uint) m->item().s_enum << m->type().name();
            *(m->var()) = JSmoke::Global::QtEnum.call(QScriptValue(), args);
            break;
        }
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_class:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
            QScriptValue value = *(m->var());

            if (value.isNull()) {
                m->item().s_class = 0;
                return;
            }
            
            if (value.isDate()) {
                Smoke::ModuleIndex classId = qtcore_Smoke->findClass(m->smoke()->classes[m->type().classId()].className);
                if (classId == JSmoke::Global::QDateClassId) {
                    m->item().s_class = new QDate(value.toDateTime().date());
                } else if (classId == JSmoke::Global::QDateTimeClassId) {
                     m->item().s_class = new QDateTime(value.toDateTime());
                } else if (classId == JSmoke::Global::QTimeClassId) {
                     m->item().s_class = new QTime(value.toDateTime().time());
                } else {
                    m->item().s_class = 0;
                }
                
                return;
            } else if (value.isRegExp()) {
                m->item().s_class = new QRegExp(value.toRegExp());
                return;
            }
            
            if (!Object::Instance::isSmokeObject(value)) {
                m->item().s_class = 0;
                return;
            }
            
            Object::Instance * instance = Object::Instance::get(value);
            void * ptr = instance->value;
            
            if (!m->cleanup() && m->type().isStack()) {
                ptr = constructCopy(instance);
            }
            
            ptr = instance->classId.smoke->cast(    ptr, 
                                                    instance->classId, 
                                                    Smoke::ModuleIndex(m->smoke(), m->type().classId()) );
            
            m->item().s_class = ptr;
            break;
        }
        
        case Marshall::ToQScriptValue:
        {
            if (m->item().s_voidp == 0) {
                *(m->var()) = m->engine()->nullValue();
                return;
            }
            
            void * ptr = m->item().s_voidp;
            QScriptValue * value = JSmoke::Global::getScriptValue(ptr);
            
            if (value != 0) {
                *(m->var()) = *value;
                return ;
            }
            
            QScriptValue obj = Global::wrapInstance(  m->engine(), 
                                        qtcore_Smoke->findClass(m->smoke()->classes[m->type().classId()].className), 
                                        ptr,
                                        QScriptEngine::QtOwnership );
            
            if (m->type().isConst() && m->type().isRef()) {
                Object::Instance * instance = Object::Instance::get(obj);
                ptr = constructCopy(instance);

                if (ptr != 0) {
                    instance->value = ptr;
                    instance->ownership = QScriptEngine::ScriptOwnership;
                    Global::mapPointer(new QScriptValue(obj), instance, instance->classId.index, 0);
                }
            }

            *(m->var()) = obj;
            break;
        }
        
        default:
            m->unsupported();
            break;
        }
        break;
        
    default:
        m->unsupported();
        break;
    }
}
    
static void marshall_void(Marshall * /*m*/) {}
static void marshall_unknown(Marshall *m) {
    m->unsupported();
}

/*
    This marshaller will work with any enum as they are all the same size in C++, 
    but use 'QLocale::Country' here to keep the compiler happy.
*/
void marshall_QListEnum(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {        
        m->item().s_voidp = new QList<QLocale::Country>(qscriptvalue_cast<QList<QLocale::Country> >(*(m->var())));
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        *(m->var()) = m->engine()->toScriptValue(*(static_cast<QList<QLocale::Country>* >(m->item().s_voidp)));
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

static QHash<QString, Marshall::TypeHandler *> TypeHandlers;

void Marshall::installHandlers(TypeHandler * handler) {
    while (handler->name != 0) {
        TypeHandlers.insert(handler->name, handler);
        handler++;
    }
}

Marshall::HandlerFn Marshall::getMarshallFn(const SmokeType &type) {
    if (type.element() != 0) {
        return marshall_basetype;
    }
    
    if (type.name() == 0) {
        return marshall_void;
    }
    
    TypeHandler * handler = TypeHandlers[type.name()];
    if (handler == 0 && type.isConst() && qstrlen(type.name()) > qstrlen("const ")) {
        handler = TypeHandlers[type.name() + qstrlen("const ")];
    }
    
    if (handler != 0) {
        return handler->fn;
    }

    return marshall_unknown;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
