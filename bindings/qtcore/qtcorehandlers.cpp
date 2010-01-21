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

static void marshall_CStringArray(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    {
        if (m->var()->isNull()) {
            m->item().s_voidp = 0;
            return;
        }
        
        quint32 length = m->var()->property(QLatin1String("length")).toUInt32();
        char **argv = new char *[length + 1];
        for (quint32 i = 0; i < length; i++) {
            QByteArray temp(m->var()->property(i).toString().toLatin1());
            argv[i] = new char[temp.length() + 1];
            strcpy(argv[i], temp.constData());
        }
        
        argv[length] = 0;
        m->item().s_voidp = argv;
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

/*
    Primitive JavaScript types are immutable, and so to marshall references to C primitives
    they need to be passed as single element JavaScript Arrays to make them mutable.
 */
template <class T>
void marshall_PrimitiveRef(Marshall *m) {
    switch(m->action()) {
    case Marshall::FromQScriptValue:
    { 
        QScriptValue array = *(m->var());
        T value;
        
        if (array.isArray()) {
            value = qscriptvalue_cast<T>(array.property(0));
        } else {
            value = qscriptvalue_cast<T>(array);
        }
        
        m->item().s_voidp = &value;        
        m->next();
        
        if (!m->type().isConst() && array.isArray()) {
            array.setProperty(0,  m->engine()->toScriptValue(*(static_cast<T*>(m->item().s_voidp))));
        }
        break;
    }
 
    case Marshall::ToQScriptValue:
    {
        QScriptValue value;
        
        if (m->type().isConst()) {
            value = m->engine()->toScriptValue(*(static_cast<T*>(m->item().s_voidp)));
        } else {
            value = m->engine()->newArray(1);
            value.setProperty(0, m->engine()->toScriptValue(*(static_cast<T*>(m->item().s_voidp))));
        }
        
        *(m->var()) = value;
        m->next();
        
        if (!m->type().isConst()) {
            *(static_cast<T*>(m->item().s_voidp)) = qscriptvalue_cast<T>(value.property(0));
        }
        break;
    }
    
    default:
        m->unsupported();
        break;
    }
}

Marshall::TypeHandler QtCoreHandlers[] = {
    { "bool*", marshall_PrimitiveRef<bool> },
    { "bool&", marshall_PrimitiveRef<bool> },
    { "char*", marshall_CString },
    { "char**", marshall_CStringArray },
    { "char&", marshall_PrimitiveRef<char> },
    { "double*", marshall_PrimitiveRef<double> },
    { "double&", marshall_PrimitiveRef<double> },
    { "int*", marshall_PrimitiveRef<int> },
    { "int&", marshall_PrimitiveRef<int> },
    { "long*", marshall_PrimitiveRef<long> },
    { "long&", marshall_PrimitiveRef<long> },
    { "QHash<int,QByteArray>", marshall_Container<QHash<int,QByteArray> > },
    { "QHash<int,QByteArray>&", marshall_Container<QHash<int,QByteArray> > },
    { "QHash<QString,QVariant>", marshall_Container<QHash<QString,QVariant> > },
    { "QHash<QString,QVariant>&", marshall_Container<QHash<QString,QVariant> > },
    { "QList<bool>", marshall_Container<QList<bool> > },
    { "QList<bool>&", marshall_Container<QList<bool> > },
    { "QList<double>", marshall_Container<QList<double> > },
    { "QList<double>&", marshall_Container<QList<double> > },
    { "QList<int>", marshall_Container<QList<int> > },
    { "QList<int>*", marshall_Container<QList<int> > },
    { "QList<int>&", marshall_Container<QList<int> > },
    { "QList<QByteArray>", marshall_Container<QList<QByteArray> > },                                                                                                  
    { "QList<QByteArray>&", marshall_Container<QList<QByteArray> > },                                                                                                  
    { "QList<QDate>", marshall_Container<QList<QDate> > },                                                                                                  
    { "QList<QDate>&", marshall_Container<QList<QDate> > },                                                                                                  
    { "QList<QDateTime>", marshall_Container<QList<QDateTime> > },                                                                                                  
    { "QList<QDateTime>&", marshall_Container<QList<QDateTime> > },                                                                                                  
    { "QList<QFileInfo>&", marshall_Container<QList<QFileInfo> > },                                                                                                   
    { "QList<QLocale::Country>", marshall_Container<QList<QLocale::Country> > },
    { "QList<QModelIndex>&", marshall_Container<QList<QModelIndex> > },                                                                                           
    { "QList<QModelIndex>&", marshall_Container<QList<QModelIndex> > },                                                                                                 
    { "QList<QObject*>", marshall_Container<QList<QObject*> > },
    { "QList<QObject*>&", marshall_Container<QList<QObject*> > },
    { "QList<QPair<int,int>>", marshall_Container<QList<QPair<int,int> > > },
    { "QList<QPair<QByteArray,QByteArray>>", marshall_Container<QList<QPair<QByteArray,QByteArray> > > },
    { "QList<QPair<QByteArray,QByteArray>>&", marshall_Container<QList<QPair<QByteArray,QByteArray> > > },
    { "QList<QPair<QString,QChar>>", marshall_Container<QList<QPair<QString,QChar> > > },
    { "QList<QPair<QString,QString>>", marshall_Container<QList<QPair<QString,QString> > > },
    { "QList<QPair<QString,QString>>&", marshall_Container<QList<QPair<QString,QString> > > },
    { "QList<QPair<QString,unsigned short>>", marshall_Container<QList<QPair<QString,unsigned short> > > },
    { "QList<qreal>", marshall_Container<QList<qreal> > },
    { "QList<qreal>&", marshall_Container<QList<qreal> > },
    { "QList<QRectF>&", marshall_Container<QList<QRectF> > },
    { "QList<QRegExp>&", marshall_Container<QList<QRegExp> > },
    { "QList<QSize>&", marshall_Container<QList<QSize> > },
    { "QList<QStringList>", marshall_Container<QList<QStringList> > },
    { "QList<QStringList>&", marshall_Container<QList<QStringList> > },
    { "QList<QTime>", marshall_Container<QList<QTime> > },                                                                                                  
    { "QList<QTime>&", marshall_Container<QList<QTime> > },                                                                                                  
    { "QList<QUrl>&", marshall_Container<QList<QUrl> > },
    { "QList<QVariant>", marshall_Container<QList<QVariant> > },
    { "QList<QVariant>&", marshall_Container<QList<QVariant> > },
    { "QList<unsigned int>", marshall_Container<QList<unsigned int> > },
    { "QList<unsigned int>&", marshall_Container<QList<unsigned int> > },
    { "qlonglong", marshall_QLongLong },
    { "qlonglong&", marshall_QLongLong },
    { "QMap<int,QVariant>", marshall_Container<QMap<int,QVariant> > },
    { "QMap<int,QVariant>&", marshall_Container<QMap<int,QVariant> > },
    { "QMap<QString,QVariant>", marshall_Container<QMap<QString,QVariant> > },
    { "QMap<QString,QVariant>&", marshall_Container<QMap<QString,QVariant> > },
    { "QStringList", marshall_Container<QStringList> },
    { "QStringList&", marshall_Container<QStringList> },
    { "QString", marshall_QString },
    { "QString*", marshall_QString },
    { "QString&", marshall_QString },
    { "qulonglong", marshall_QULongLong },
    { "qulonglong&", marshall_QULongLong },
    { "QVector<double>", marshall_Container<QVector<qreal> > },
    { "QVector<double>&", marshall_Container<QVector<qreal> > },
    { "QVector<QLineF>&", marshall_Container<QVector<QLineF> > },
    { "QVector<QLine>&", marshall_Container<QVector<QLine> > },
    { "QVector<QPair<double,QVariant>>", marshall_Container<QVector<QPair<double,QVariant> > > },
    { "QVector<QPair<double,QVariant>>&", marshall_Container<QVector<QPair<double,QVariant> > > },
    { "QVector<QPointF>&", marshall_Container<QVector<QPointF> > },
    { "QVector<QPoint>", marshall_Container<QVector<QPoint> > },
    { "QVector<QPoint>&", marshall_Container<QVector<QPoint> > },
    { "QVector<qreal>", marshall_Container<QVector<qreal> > },
    { "QVector<qreal>&", marshall_Container<QVector<qreal> > },
    { "QVector<QRectF>&", marshall_Container<QVector<QRectF> > },
    { "QVector<QRect>", marshall_Container<QVector<QRect> > },
    { "QVector<QRect>&", marshall_Container<QVector<QRect> > },
    { "QVector<QVariant>&", marshall_Container<QVector<QVariant> > },
    { "QVector<QXmlStreamEntityDeclaration>", marshall_Container<QVector<QXmlStreamEntityDeclaration> > },
    { "QVector<QXmlStreamEntityDeclaration>&", marshall_Container<QVector<QXmlStreamEntityDeclaration> > },
    { "QVector<QXmlStreamNamespaceDeclaration>", marshall_Container<QVector<QXmlStreamNamespaceDeclaration> > },
    { "QVector<QXmlStreamNamespaceDeclaration>&", marshall_Container<QVector<QXmlStreamNamespaceDeclaration> > },
    { "QVector<QXmlStreamNotationDeclaration>", marshall_Container<QVector<QXmlStreamNotationDeclaration> > },
    { "QVector<QXmlStreamNotationDeclaration>&", marshall_Container<QVector<QXmlStreamNotationDeclaration> > },
    { "QVector<unsigned int>", marshall_Container<QVector<unsigned int> > },
    { "QVector<unsigned int>&", marshall_Container<QVector<unsigned int> > },
    { "short*", marshall_PrimitiveRef<short> },
    { "short&", marshall_PrimitiveRef<short> },
    { "signed int&", marshall_PrimitiveRef<int> },
    { "signed long&", marshall_PrimitiveRef<long> },
    { "signed short&", marshall_PrimitiveRef<short> },
    { "unsigned char*", marshall_CString },
    { "unsigned char&", marshall_PrimitiveRef<unsigned char> },
    { "unsigned int&", marshall_PrimitiveRef<unsigned int> },
    { "unsigned long&", marshall_PrimitiveRef<unsigned long> },


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
