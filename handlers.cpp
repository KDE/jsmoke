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


#include <QtCore/QByteArray>                                                                                                              
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>                                                                                                               
#include <QtCore/QLine>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QModelIndex>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
// #include <QtCore/QPrinterInfo>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QSize>
#include <QtCore/QTime>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#include <QtGui/QColor>
#include <QtGui/QImageTextKeyLang>                                                                                                       
#include <QtGui/QKeySequence>
#include <QtGui/QPolygon>
#include <QtGui/QTableWidgetSelectionRange>
#include <QtGui/QTextBlock>
#include <QtGui/QTextFormat>
#include <QtGui/QTextLength>

// #include <QtNetork/QSslError>
#include <QtNetwork/QHostAddress>                                                                                                            
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>


#include "marshall.h"
#include "global.h"
#include "qtscript-smoke.h"
#include "SmokeQtScriptUtils.h"
#include "marshall_macros.h"


Q_DECLARE_METATYPE(QLocale::Country)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(QFileInfo)
Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(QImageTextKeyLang)
Q_DECLARE_METATYPE(QModelIndex)
Q_DECLARE_METATYPE(QNetworkAddressEntry)
Q_DECLARE_METATYPE(QNetworkInterface)
Q_DECLARE_METATYPE(QNetworkProxy)
Q_DECLARE_METATYPE(QPolygonF)
Q_DECLARE_METATYPE(QSslCertificate)
Q_DECLARE_METATYPE(QSslCipher)
Q_DECLARE_METATYPE(QTableWidgetSelectionRange)
Q_DECLARE_METATYPE(QTextBlock)

Q_DECLARE_METATYPE(QList<QLocale::Country>)
Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(QList<qreal>)
Q_DECLARE_METATYPE(QList<QByteArray>)                                                                                                              
Q_DECLARE_METATYPE(QList<QFileInfo>)                                                                                                              
Q_DECLARE_METATYPE(QList<QHostAddress>)                                                                                                          
Q_DECLARE_METATYPE(QList<QImageTextKeyLang>)                                                                                                       
Q_DECLARE_METATYPE(QList<QKeySequence>)
Q_DECLARE_METATYPE(QList<QModelIndex>)
Q_DECLARE_METATYPE(QList<QNetworkAddressEntry>)
//Q_DECLARE_METATYPE(QList<QNetworkCookie>)
Q_DECLARE_METATYPE(QList<QNetworkInterface>)
Q_DECLARE_METATYPE(QList<QNetworkProxy>)
Q_DECLARE_METATYPE(QList<QPolygonF>)
// Q_DECLARE_METATYPE(QList<QPrinterInfo>)
Q_DECLARE_METATYPE(QList<QRectF>)
Q_DECLARE_METATYPE(QList<QSize>)
Q_DECLARE_METATYPE(QList<QSslCertificate>)
Q_DECLARE_METATYPE(QList<QSslCipher>)
// Q_DECLARE_METATYPE(QList<QSslError>)
Q_DECLARE_METATYPE(QList<QTableWidgetSelectionRange>)
Q_DECLARE_METATYPE(QList<QTextBlock>)
Q_DECLARE_METATYPE(QList<QUrl>)
Q_DECLARE_METATYPE(QList<QVariant>)

Q_DECLARE_METATYPE(QVector<QColor>)
Q_DECLARE_METATYPE(QVector<QLine>)
Q_DECLARE_METATYPE(QVector<QLineF>)
Q_DECLARE_METATYPE(QVector<QPoint>)
Q_DECLARE_METATYPE(QVector<QPointF>)
Q_DECLARE_METATYPE(QVector<QRect>)
Q_DECLARE_METATYPE(QVector<QRectF>)
Q_DECLARE_METATYPE(QVector<QTextFormat>)
Q_DECLARE_METATYPE(QVector<QTextLength>)
Q_DECLARE_METATYPE(QVector<QVariant>)
Q_DECLARE_METATYPE(QVector<unsigned int>)
Q_DECLARE_METATYPE(QVector<qreal>)

namespace QtScriptSmoke {

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

/*
    This marshaller will work with any enum as they are all the same size in C++, 
    but use 'QLocale::Country' here to keep the compiler happy.
*/
static void marshall_QListEnum(Marshall *m) {
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

DEF_CONTAINER_MARSHALLER(QListQReal, QList<qreal>)
DEF_CONTAINER_MARSHALLER(QListInt, QList<int>)

DEF_CONTAINER_MARSHALLER(QListQByteArray, QList<QByteArray>)                                                                                                             
DEF_CONTAINER_MARSHALLER(QListQFileInfo, QList<QFileInfo>)                                                                                                               
DEF_CONTAINER_MARSHALLER(QListQHostAddress, QList<QHostAddress>)                                                                                                            
DEF_CONTAINER_MARSHALLER(QListQImageTextKeyLang, QList<QImageTextKeyLang>)                                                                                                      
DEF_CONTAINER_MARSHALLER(QListQKeySequence, QList<QKeySequence>)
DEF_CONTAINER_MARSHALLER(QListQModelIndex, QList<QModelIndex>)
DEF_CONTAINER_MARSHALLER(QListQNetworkAddressEntry, QList<QNetworkAddressEntry>)
DEF_CONTAINER_MARSHALLER(QListQNetworkCookie, QList<QNetworkCookie>)
DEF_CONTAINER_MARSHALLER(QListQNetworkInterface, QList<QNetworkInterface>)
DEF_CONTAINER_MARSHALLER(QListQNetworkProxy, QList<QNetworkProxy>)
DEF_CONTAINER_MARSHALLER(QListQPolygonF, QList<QPolygonF>)
// DEF_CONTAINER_MARSHALLER(QListQPrinterInfo, QList<QPrinterInfo>)
DEF_CONTAINER_MARSHALLER(QListQRectF, QList<QRectF>)
DEF_CONTAINER_MARSHALLER(QListQSize, QList<QSize>)
DEF_CONTAINER_MARSHALLER(QListQSslCertificate, QList<QSslCertificate>)
// DEF_CONTAINER_MARSHALLER(QListQSslCipher, QList<QSslCipher>)
// DEF_CONTAINER_MARSHALLER(QListQSslError, QList<QSslError>
DEF_CONTAINER_MARSHALLER(QListQTableWidgetSelectionRange, QList<QTableWidgetSelectionRange>)
DEF_CONTAINER_MARSHALLER(QListQTextBlock, QList<QTextBlock>)
DEF_CONTAINER_MARSHALLER(QListQUrl, QList<QUrl>)
DEF_CONTAINER_MARSHALLER(QListQVariant, QList<QVariant>)

DEF_CONTAINER_MARSHALLER(QVectorQReal, QVector<qreal>)
DEF_CONTAINER_MARSHALLER(QVectorUInt, QVector<unsigned int>)
DEF_CONTAINER_MARSHALLER(QVectorQColor, QVector<QColor>)
DEF_CONTAINER_MARSHALLER(QVectorQLine, QVector<QLine>)
DEF_CONTAINER_MARSHALLER(QVectorQLineF, QVector<QLineF>)
DEF_CONTAINER_MARSHALLER(QVectorQPoint, QVector<QPoint>)
DEF_CONTAINER_MARSHALLER(QVectorQPointF, QVector<QPointF>)
DEF_CONTAINER_MARSHALLER(QVectorQRect, QVector<QRect>)
DEF_CONTAINER_MARSHALLER(QVectorQRectF, QVector<QRectF>)
DEF_CONTAINER_MARSHALLER(QVectorQTextFormat, QVector<QTextFormat>)
DEF_CONTAINER_MARSHALLER(QVectorQTextLength, QVector<QTextLength>)
DEF_CONTAINER_MARSHALLER(QVectorQVariant, QVector<QVariant>)

TypeHandler Handlers[] = {
    { "QList<int>", marshall_QListInt },
    { "QList<int>*", marshall_QListInt },
    { "QList<int>&", marshall_QListInt },
    { "QList<QFontDatabase::WritingSystem>", marshall_QListEnum },
    { "QList<QLocale::Country>", marshall_QListEnum },
    { "QList<QPrinter::PageSize>", marshall_QListEnum },
    { "QList<qreal>", marshall_QListQReal },
    { "QList<qreal>&", marshall_QListQReal },
    { "QList<QTextOption::Tab>", marshall_QListEnum },
    { "QList<QTextOption::Tab>&", marshall_QListEnum },
    { "QList<QWizard::WizardButton>&", marshall_QListEnum },
    { "QListView::Flow", marshall_QListEnum },
    { "QListView::LayoutMode", marshall_QListEnum },
    { "QListView::Movement", marshall_QListEnum },
    { "QListView::ResizeMode", marshall_QListEnum },
    { "QListView::ViewMode", marshall_QListEnum },
    { "QListWidgetItem::ItemType", marshall_QListEnum },
    { "QString", marshall_QString },
    { "QString*", marshall_QString },
    { "QString&", marshall_QString },
    { "QVector<QColor>", marshall_QVectorQColor },
    { "QVector<QLineF>&", marshall_QVectorQLineF },
    { "QVector<QLine>&", marshall_QVectorQLine },
    { "QVector<QPointF>&", marshall_QVectorQPointF },
    { "QVector<QPoint>", marshall_QVectorQPoint },
    { "QVector<QPoint>&", marshall_QVectorQPoint },
    { "QVector<qreal>", marshall_QVectorQReal },
    { "QVector<qreal>&", marshall_QVectorQReal },
    { "QVector<QRectF>&", marshall_QVectorQRectF },
    { "QVector<QRect>", marshall_QVectorQRect },
    { "QVector<QRect>&", marshall_QVectorQRect },
    { "QVector<QTextFormat>", marshall_QVectorQTextFormat },
    { "QVector<QTextLength>", marshall_QVectorQTextLength },
    { "QVector<QTextLength>&", marshall_QVectorQTextLength },
    { "QVector<QVariant>&", marshall_QVectorQVariant },
    { "QVector<unsigned int>", marshall_QVectorUInt },
    { "QVector<unsigned int>&", marshall_QVectorUInt },

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

void registerTypes(QScriptEngine * engine) {   
    qScriptRegisterSequenceMetaType<QList<QLocale::Country> >(engine); 
    qScriptRegisterSequenceMetaType<QList<int> >(engine); 
    qScriptRegisterSequenceMetaType<QList<qreal> >(engine); 
    
    qScriptRegisterSequenceMetaType<QVector<unsigned int> >(engine); 
    qScriptRegisterSequenceMetaType<QVector<qreal> >(engine); 
    
    qScriptSmokeRegisterSequenceMetaType<QVector<QColor> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QLine> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QLineF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QPoint> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QPointF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QRect> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QRectF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QTextFormat> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QTextLength> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QVariant> >(engine);

    qScriptSmokeRegisterSequenceMetaType<QList<QByteArray> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QFileInfo> >(engine);                                                                                                              
    qScriptSmokeRegisterSequenceMetaType<QList<QHostAddress> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QImageTextKeyLang> >(engine);                                                                                                       
    qScriptSmokeRegisterSequenceMetaType<QList<QKeySequence> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QModelIndex> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkAddressEntry> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkCookie> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkInterface> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkProxy> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QPolygonF> >(engine);
//    qScriptSmokeRegisterSequenceMetaType<QList<QPrinterInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QRectF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSize> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslCertificate> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslCipher> >(engine);
//    qScriptSmokeRegisterSequenceMetaType<QList<QSslError> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTableWidgetSelectionRange> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTextBlock> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QUrl> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QVariant> >(engine);

}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
