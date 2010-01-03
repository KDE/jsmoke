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
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTime>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QXmlStreamEntityDeclaration>
#include <QtCore/QXmlStreamNamespaceDeclaration>
#include <QtCore/QXmlStreamNotationDeclaration>

#include <global.h>
#include <marshall.h>
#include <marshallmacros.h>
#include <object.h>

Q_DECLARE_METATYPE2(QHash<int,QByteArray>)
Q_DECLARE_METATYPE2(QHash<QString,QVariant>)
Q_DECLARE_METATYPE2(QList<QPair<int,int> >)
Q_DECLARE_METATYPE2(QList<QPair<QByteArray,QByteArray> >)
Q_DECLARE_METATYPE2(QList<QPair<QString,QChar> >)
Q_DECLARE_METATYPE2(QList<QPair<QString,QString> >)
Q_DECLARE_METATYPE2(QList<QPair<QString,unsigned short> >)
Q_DECLARE_METATYPE2(QMap<int,QVariant>)
Q_DECLARE_METATYPE2(QMap<QString,QVariant>)
Q_DECLARE_METATYPE2(QVector<QPair<double,QVariant> >)
Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(QDate)
Q_DECLARE_METATYPE(QDateTime)
Q_DECLARE_METATYPE(QFileInfo)
Q_DECLARE_METATYPE(QList<bool>)
Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(QList<unsigned int>)
Q_DECLARE_METATYPE(QList<QByteArray>)                                                                                                              
Q_DECLARE_METATYPE(QList<QDate>)                                                                                                              
Q_DECLARE_METATYPE(QList<QDateTime>)                                                                                                              
Q_DECLARE_METATYPE(QList<QFileInfo>)                                                                                                              
Q_DECLARE_METATYPE(QList<QLocale::Country>)
Q_DECLARE_METATYPE(QList<QModelIndex>)
Q_DECLARE_METATYPE(QList<QObject*>)
Q_DECLARE_METATYPE(QList<qreal>)
Q_DECLARE_METATYPE(QList<QRectF>)
Q_DECLARE_METATYPE(QList<QRegExp>)
Q_DECLARE_METATYPE(QList<QSize>)
Q_DECLARE_METATYPE(QList<QStringList>)
Q_DECLARE_METATYPE(QList<QTime>)                                                                                                              
Q_DECLARE_METATYPE(QList<QUrl>)
Q_DECLARE_METATYPE(QList<QVariant>)
Q_DECLARE_METATYPE(QLocale::Country)
Q_DECLARE_METATYPE(QModelIndex)
Q_DECLARE_METATYPE(QRegExp)
Q_DECLARE_METATYPE(QTime)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(QVector<QLine>)
Q_DECLARE_METATYPE(QVector<QLineF>)
Q_DECLARE_METATYPE(QVector<QPoint>)
Q_DECLARE_METATYPE(QVector<QPointF>)
Q_DECLARE_METATYPE(QVector<qreal>)
Q_DECLARE_METATYPE(QVector<QRect>)
Q_DECLARE_METATYPE(QVector<QRectF>)
Q_DECLARE_METATYPE(QVector<QVariant>)
Q_DECLARE_METATYPE(QVector<QXmlStreamEntityDeclaration>)
Q_DECLARE_METATYPE(QVector<QXmlStreamNamespaceDeclaration>)
Q_DECLARE_METATYPE(QVector<QXmlStreamNotationDeclaration>)
Q_DECLARE_METATYPE(QVector<unsigned int>)
Q_DECLARE_METATYPE(QXmlStreamEntityDeclaration)
Q_DECLARE_METATYPE(QXmlStreamNamespaceDeclaration)
Q_DECLARE_METATYPE(QXmlStreamNotationDeclaration)

namespace JSmoke {

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

static void marshall_BoolPtr(Marshall *m) {
    switch(m->action()) {            
    case Marshall::FromQScriptValue:
    {
        m->item().s_voidp = new bool;
        *static_cast<bool*>(m->item().s_voidp) = (*(m->var())).toBool();
        m->next();
        delete static_cast<bool*>(m->item().s_voidp);
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        *(m->var()) = QScriptValue(m->engine(), *static_cast<bool*>(m->item().s_voidp));
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

static void marshall_CString(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {
        if (m->var()->isNull()) {
            m->item().s_voidp = 0;
            return;
        }
        
        QByteArray temp(m->var()->toString().toLatin1());
        m->item().s_voidp = qstrdup(temp.constData());
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        if (m->item().s_voidp == 0) {
            *(m->var()) = m->engine()->nullValue();
            return;
        }
        
        *(m->var()) = QScriptValue(m->engine(), static_cast<char*>(m->item().s_voidp));
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

static void marshall_QLongLong(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {        
        m->item().s_voidp = new qlonglong((*(m->var())).toInteger());
        m->next();
        
        if (m->cleanup() && m->type().isConst()) {
            delete static_cast<qlonglong*>(m->item().s_voidp);
        }        
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        *(m->var()) = QScriptValue(m->engine(), qreal(*static_cast<qlonglong*>(m->item().s_voidp)));
        m->next();
        
        if (m->cleanup() && m->type().isConst()) {
            delete static_cast<qlonglong*>(m->item().s_voidp);
        }        
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

static void marshall_QULongLong(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {        
        m->item().s_voidp = new qulonglong((*(m->var())).toInteger());
        m->next();
        
        if (m->cleanup() && m->type().isConst()) {
            delete static_cast<qulonglong*>(m->item().s_voidp);
        }        
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        *(m->var()) = QScriptValue(m->engine(), qreal(*static_cast<qulonglong*>(m->item().s_voidp)));
        m->next();
        
        if (m->cleanup() && m->type().isConst()) {
            delete static_cast<qulonglong*>(m->item().s_voidp);
        }        
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

DEF_CONTAINER_MARSHALLER3(QHashIntQByteArray, QHash<int,QByteArray>)
DEF_CONTAINER_MARSHALLER3(QHashQStringQVariant, QHash<QString,QVariant>)
DEF_CONTAINER_MARSHALLER3(QListQPairIntInt, QList<QPair<int,int> >)
DEF_CONTAINER_MARSHALLER3(QListQPairQByteArrayQByteArray, QList<QPair<QByteArray,QByteArray> >)
DEF_CONTAINER_MARSHALLER3(QListQPairQStringQChar, QList<QPair<QString,QChar> >)
DEF_CONTAINER_MARSHALLER3(QListQPairQStringQString, QList<QPair<QString,QString> >)
DEF_CONTAINER_MARSHALLER3(QListQPairQStringUShort, QList<QPair<QString,unsigned short> >)
DEF_CONTAINER_MARSHALLER3(QMapIntQVariant, QMap<int,QVariant>)
DEF_CONTAINER_MARSHALLER3(QMapQStringQVariant, QMap<QString,QVariant>)
DEF_CONTAINER_MARSHALLER3(QVectorQPairDoubleQVariant, QVector<QPair<double,QVariant> >)
DEF_CONTAINER_MARSHALLER(QListBool, QList<bool>)
DEF_CONTAINER_MARSHALLER(QListDouble, QList<double>)
DEF_CONTAINER_MARSHALLER(QListInt, QList<int>)
DEF_CONTAINER_MARSHALLER(QListUInt, QList<unsigned int>)
DEF_CONTAINER_MARSHALLER(QListQByteArray, QList<QByteArray>)                                                                                                             
DEF_CONTAINER_MARSHALLER(QListQDate, QList<QDate>)                                                                                                             
DEF_CONTAINER_MARSHALLER(QListQDateTime, QList<QDateTime>)                                                                                                             
DEF_CONTAINER_MARSHALLER(QListQFileInfo, QList<QFileInfo>)                                                                                                               
DEF_CONTAINER_MARSHALLER(QListQModelIndex, QList<QModelIndex>)
DEF_CONTAINER_MARSHALLER(QListQObject, QList<QObject*>)
DEF_CONTAINER_MARSHALLER(QListQReal, QList<qreal>)
DEF_CONTAINER_MARSHALLER(QListQRectF, QList<QRectF>)
DEF_CONTAINER_MARSHALLER(QListQRegExp, QList<QRegExp>)
DEF_CONTAINER_MARSHALLER(QListQSize, QList<QSize>)
DEF_CONTAINER_MARSHALLER(QListQStringList, QList<QStringList>)
DEF_CONTAINER_MARSHALLER(QListQTime, QList<QTime>)                                                                                                             
DEF_CONTAINER_MARSHALLER(QListQUrl, QList<QUrl>)
DEF_CONTAINER_MARSHALLER(QListQVariant, QList<QVariant>)
DEF_CONTAINER_MARSHALLER(QStringList, QStringList)
DEF_CONTAINER_MARSHALLER(QVectorQLineF, QVector<QLineF>)
DEF_CONTAINER_MARSHALLER(QVectorQLine, QVector<QLine>)
DEF_CONTAINER_MARSHALLER(QVectorQPointF, QVector<QPointF>)
DEF_CONTAINER_MARSHALLER(QVectorQPoint, QVector<QPoint>)
DEF_CONTAINER_MARSHALLER(QVectorQReal, QVector<qreal>)
DEF_CONTAINER_MARSHALLER(QVectorQRectF, QVector<QRectF>)
DEF_CONTAINER_MARSHALLER(QVectorQRect, QVector<QRect>)
DEF_CONTAINER_MARSHALLER(QVectorQVariant, QVector<QVariant>)
DEF_CONTAINER_MARSHALLER(QVectorQXmlStreamEntityDeclaration, QVector<QXmlStreamEntityDeclaration>)
DEF_CONTAINER_MARSHALLER(QVectorQXmlStreamNamespaceDeclaration, QVector<QXmlStreamNamespaceDeclaration>)
DEF_CONTAINER_MARSHALLER(QVectorQXmlStreamNotationDeclaration, QVector<QXmlStreamNotationDeclaration>)
DEF_CONTAINER_MARSHALLER(QVectorUInt, QVector<unsigned int>)

Marshall::TypeHandler QtCoreHandlers[] = {
    { "bool*", marshall_BoolPtr },
    { "char*", marshall_CString },
    { "QHash<int,QByteArray>", marshall_QHashIntQByteArray },
    { "QHash<int,QByteArray>&", marshall_QHashIntQByteArray },
    { "QHash<QString,QVariant>", marshall_QHashQStringQVariant },
    { "QHash<QString,QVariant>&", marshall_QHashQStringQVariant },
    { "QList<bool>", marshall_QListBool },
    { "QList<bool>&", marshall_QListBool },
    { "QList<double>", marshall_QListDouble },
    { "QList<double>&", marshall_QListDouble },
    { "QList<int>", marshall_QListInt },
    { "QList<int>*", marshall_QListInt },
    { "QList<int>&", marshall_QListInt },
    { "QList<unsigned int>", marshall_QListUInt },
    { "QList<unsigned int>&", marshall_QListUInt },
    { "QList<QByteArray>", marshall_QListQByteArray },                                                                                                  
    { "QList<QByteArray>&", marshall_QListQByteArray },                                                                                                  
    { "QList<QDate>", marshall_QListQDate },                                                                                                  
    { "QList<QDate>&", marshall_QListQDate },                                                                                                  
    { "QList<QDateTime>", marshall_QListQDateTime },                                                                                                  
    { "QList<QDateTime>&", marshall_QListQDateTime },                                                                                                  
    { "QList<QFileInfo>&", marshall_QListQFileInfo },                                                                                                   
    { "QList<QLocale::Country>", marshall_QListEnum },
    { "QList<QModelIndex>&", marshall_QListQModelIndex },                                                                                           
    { "QList<QModelIndex>&", marshall_QListQModelIndex },                                                                                                 
    { "QList<QObject*>", marshall_QListQObject },
    { "QList<QObject*>&", marshall_QListQObject },
    { "QList<QPair<int,int>>", marshall_QListQPairIntInt },
    { "QList<QPair<QByteArray,QByteArray>>", marshall_QListQPairQByteArrayQByteArray },
    { "QList<QPair<QByteArray,QByteArray>>&", marshall_QListQPairQByteArrayQByteArray },
    { "QList<QPair<QString,unsigned short>>", marshall_QListQPairQStringUShort },
    { "QList<QPair<QString,QChar>>", marshall_QListQPairQStringQChar },
    { "QList<QPair<QString,QString>>", marshall_QListQPairQStringQString },
    { "QList<QPair<QString,QString>>&", marshall_QListQPairQStringQString },
    { "QList<qreal>", marshall_QListQReal },
    { "QList<qreal>&", marshall_QListQReal },
    { "QList<QRectF>&", marshall_QListQRectF },
    { "QList<QRegExp>&", marshall_QListQRegExp },
    { "QList<QSize>&", marshall_QListQSize },
    { "QList<QStringList>", marshall_QListQStringList },
    { "QList<QStringList>&", marshall_QListQStringList },
    { "QList<QTime>", marshall_QListQTime },                                                                                                  
    { "QList<QTime>&", marshall_QListQTime },                                                                                                  
    { "QList<QUrl>&", marshall_QListQUrl },
    { "QList<QVariant>", marshall_QListQVariant },
    { "QList<QVariant>&", marshall_QListQVariant },
    { "qlonglong", marshall_QLongLong },
    { "qlonglong&", marshall_QLongLong },
    { "QMap<int,QVariant>", marshall_QMapIntQVariant },
    { "QMap<int,QVariant>&", marshall_QMapIntQVariant },
    { "QMap<QString,QVariant>", marshall_QMapQStringQVariant },
    { "QMap<QString,QVariant>&", marshall_QMapQStringQVariant },
    { "QStringList", marshall_QStringList },
    { "QStringList&", marshall_QStringList },
    { "QString", marshall_QString },
    { "QString*", marshall_QString },
    { "QString&", marshall_QString },
    { "qulonglong", marshall_QULongLong },
    { "qulonglong&", marshall_QULongLong },
    { "QVector<double>", marshall_QVectorQReal },
    { "QVector<double>&", marshall_QVectorQReal },
    { "QVector<QLineF>&", marshall_QVectorQLineF },
    { "QVector<QLine>&", marshall_QVectorQLine },
    { "QVector<QPair<double,QVariant>>", marshall_QVectorQPairDoubleQVariant },
    { "QVector<QPair<double,QVariant>>&", marshall_QVectorQPairDoubleQVariant },
    { "QVector<QPointF>&", marshall_QVectorQPointF },
    { "QVector<QPoint>", marshall_QVectorQPoint },
    { "QVector<QPoint>&", marshall_QVectorQPoint },
    { "QVector<qreal>", marshall_QVectorQReal },
    { "QVector<qreal>&", marshall_QVectorQReal },
    { "QVector<QRectF>&", marshall_QVectorQRectF },
    { "QVector<QRect>", marshall_QVectorQRect },
    { "QVector<QRect>&", marshall_QVectorQRect },
    { "QVector<QVariant>&", marshall_QVectorQVariant },
    { "QVector<QXmlStreamEntityDeclaration>", marshall_QVectorQXmlStreamEntityDeclaration },
    { "QVector<QXmlStreamEntityDeclaration>&", marshall_QVectorQXmlStreamEntityDeclaration },
    { "QVector<QXmlStreamNamespaceDeclaration>", marshall_QVectorQXmlStreamNamespaceDeclaration },
    { "QVector<QXmlStreamNamespaceDeclaration>&", marshall_QVectorQXmlStreamNamespaceDeclaration },
    { "QVector<QXmlStreamNotationDeclaration>", marshall_QVectorQXmlStreamNotationDeclaration },
    { "QVector<QXmlStreamNotationDeclaration>&", marshall_QVectorQXmlStreamNotationDeclaration },
    { "QVector<unsigned int>", marshall_QVectorUInt },
    { "QVector<unsigned int>&", marshall_QVectorUInt },
    { "unsigned char*", marshall_CString },

    { 0, 0 }
};

void registerQtCoreTypes(QScriptEngine * engine) 
{     
    qScriptRegisterSequenceMetaType<QList<QStringList> >(engine); 
    qScriptRegisterSequenceMetaType<QList<bool> >(engine); 
    qScriptRegisterSequenceMetaType<QList<double> >(engine); 
    qScriptRegisterSequenceMetaType<QList<int> >(engine); 
    qScriptRegisterSequenceMetaType<QList<unsigned int> >(engine); 
    qScriptRegisterSequenceMetaType<QList<QLocale::Country> >(engine); 
    qScriptRegisterSequenceMetaType<QList<qreal> >(engine); 
    qScriptRegisterSequenceMetaType<QVector<qreal> >(engine); 
    qScriptRegisterSequenceMetaType<QVector<unsigned int> >(engine); 
    qScriptSmokeRegisterHashMetaType<QHash<int,QByteArray> >(engine);
    qScriptSmokeRegisterHashMetaType<QHash<QString,QVariant> >(engine);
    qScriptSmokeRegisterHashMetaType<QMap<int,QVariant> >(engine);
    qScriptSmokeRegisterHashMetaType<QMap<QString,QVariant> >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<int,int> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<QByteArray,QByteArray> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<QString,unsigned short> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<QString,QChar> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<QString,QString> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QVector<QPair<double,QVariant> > >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QObject*> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QByteArray> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QDate> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QDateTime> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QFileInfo> >(engine);                                                                                                              
    qScriptSmokeRegisterSequenceMetaType<QList<QModelIndex> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QRectF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QRegExp> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSize> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTime> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QUrl> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QVariant> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QLine> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QLineF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QPoint> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QPointF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QRect> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QRectF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QXmlStreamEntityDeclaration> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QXmlStreamNamespaceDeclaration> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QXmlStreamNotationDeclaration> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
