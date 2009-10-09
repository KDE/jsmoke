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
#include <QtCore/QTime>
#include <QtCore/QPoint>
#include <QtCore/QVector>

#include "marshall.h"
#include "global.h"
#include "qtscript-smoke.h"
#include "SmokeQtScriptUtils.h"
#include "marshall_macros.h"

Q_DECLARE_METATYPE(QVector<QPoint>)

namespace QtScriptSmoke {
	
void
marshall_basetype(Marshall *m)
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
            } else {
                m->item().s_enum = value.toUInt32();
            }
            break;
        }
        case Marshall::ToQScriptValue:
            *(m->var()) = QScriptValue(m->engine(), (uint) m->item().s_enum);
            break;
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
                if (    m->type().classId() == QtScriptSmoke::Global::QDateClassId.index
                        && m->type().smoke() == QtScriptSmoke::Global::QDateClassId.smoke ) 
                {
                    m->item().s_class = new QDate(value.toDateTime().date());
                } else if ( m->type().classId() == QtScriptSmoke::Global::QDateTimeClassId.index
                            && m->type().smoke() == QtScriptSmoke::Global::QDateTimeClassId.smoke ) 
                {
                    m->item().s_class = new QDateTime(value.toDateTime());
                } else if ( m->type().classId() == QtScriptSmoke::Global::QTimeClassId.index
                            && m->type().smoke() == QtScriptSmoke::Global::QTimeClassId.smoke ) 
                {
                    m->item().s_class = new QTime(value.toDateTime().time());
                } else {
                    m->item().s_class = 0;
                }
                return;
            }
            
            if (!QtScriptSmoke::Instance::isSmokeObject(value)) {
                m->item().s_class = 0;
                return;
            }
            
            QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(value);
            void * ptr = instance->value;
            
            if (!m->cleanup() && m->type().isStack()) {
                ptr = constructCopy(instance);
            }
            
            if (instance->classId.smoke == m->smoke()) {
                ptr = instance->classId.smoke->cast(ptr, instance->classId.index, m->type().classId() );
            } else {
                // If the method's class and the instance's class are in different smoke modules
                // then we need to convert them both to be class ids in the instance's module in
                // order to do the cast
                const Smoke::Class &klass = m->smoke()->classes[m->type().classId()];
                ptr = instance->classId.smoke->cast(    ptr, 
                                                        instance->classId.index, 
                                                        instance->classId.smoke->idClass(klass.className, true).index );            
            }
            
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

            /* Is this needed - should QtScript Dates be returned instead of QDateTime here?
            if (    m->type().classId() == QtScriptSmoke::Global::QDateClassId.index
                    && m->type().smoke() == QtScriptSmoke::Global::QDateClassId.smoke ) 
            {
                *(m->var()) = m->engine()->newDate(QDateTime(*(static_cast<QDate*>(ptr))));
                return;
            } else if ( m->type().classId() == QtScriptSmoke::Global::QDateTimeClassId.index
                        && m->type().smoke() == QtScriptSmoke::Global::QDateTimeClassId.smoke ) 
            {
                *(m->var()) = m->engine()->newDate(*(static_cast<QDateTime*>(ptr)));
                return;
            } else if ( m->type().classId() == QtScriptSmoke::Global::QTimeClassId.index
                        && m->type().smoke() == QtScriptSmoke::Global::QTimeClassId.smoke ) 
            {
                *(m->var()) = m->engine()->newDate(QDateTime(QDate(), *(static_cast<QTime*>(ptr))));
                return;
            }
            */
            
            QScriptValue * value = QtScriptSmoke::Global::getScriptValue(ptr);
            if (value != 0) {
                *(m->var()) = *value;
                return ;
            }
            
            QtScriptSmoke::Instance * instance = new QtScriptSmoke::Instance();
            instance->classId.smoke = m->smoke();
            instance->classId.index = m->type().classId();
            instance->value = ptr;
            instance->ownership = QScriptEngine::QtOwnership;
            
            if (m->type().isConst() && m->type().isRef()) {
                ptr = constructCopy(instance);

                if (ptr != 0) {
                    instance->value = ptr;
                    instance->ownership = QScriptEngine::ScriptOwnership;
                }
            }
            
            QScriptValue obj = m->engine()->newObject(QtScriptSmoke::Global::Object); 
            QtScriptSmoke::Instance::set(obj, instance);
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

static void marshall_QString(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {
        QString * s = 0;
        
        if (m->var()->isNull()) {
            s = new QString();
        } else {
            s = new QString(m->var()->toString());
        }

        m->item().s_voidp = s;
        m->next();
    
        if (!m->type().isConst() && m->var()->isNull() && s != 0 && !s->isNull()) {
            // Copy the string back to the QScriptValue instance
        }

        if (s != 0 && m->cleanup()) {
            delete s;
        }
        
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        if (m->item().s_voidp == 0) {
            *(m->var()) = m->engine()->nullValue();
            return;
        }
        
        *(m->var()) = QScriptValue(m->engine(), *(static_cast<QString*>(m->item().s_voidp)));
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

static void marshall_QVectorQPoint(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {        
        if (m->var()->isNull()) {
            m->item().s_voidp = 0;
            return;
        }

        m->item().s_voidp = new QVector<QPoint>(qscriptvalue_cast<QVector<QPoint> >(*(m->var())));
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        if (m->item().s_voidp == 0) {
            *(m->var()) = m->engine()->nullValue();
            return;
        }
        
        *(m->var()) = m->engine()->toScriptValue(*(static_cast<QVector<QPoint>* >(m->item().s_voidp)));
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

TypeHandler Handlers[] = {
    { "QString", marshall_QString },
    { "QString*", marshall_QString },
    { "QString&", marshall_QString },
    { "QVector<QPoint>", marshall_QVectorQPoint },
    { "QVector<QPoint>&", marshall_QVectorQPoint },

    { 0, 0 }
};

QHash<QString, TypeHandler *> TypeHandlers;

void installHandlers(TypeHandler * handler) {
    while (handler->name != 0) {
        TypeHandlers.insert(handler->name, handler);
        handler++;
    }
}

Marshall::HandlerFn getMarshallFn(const SmokeType &type) {
    if (type.element() != 0) {
        return marshall_basetype;
    }
    
    if (type.name() == 0) {
        return marshall_void;
    }
    
    TypeHandler * handler = TypeHandlers[type.name()];
    if (handler == 0 && type.isConst() && strlen(type.name()) > strlen("const ")) {
        handler = TypeHandlers[type.name() + strlen("const ")];
    }
    
    if (handler != 0) {
        return handler->fn;
    }

    return marshall_unknown;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
