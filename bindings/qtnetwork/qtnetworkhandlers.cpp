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

#include <QtNetwork/QHostAddress>                                                                                                            
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QSslError>

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

Q_DECLARE_METATYPE(QHostAddress)
Q_DECLARE_METATYPE(QNetworkAddressEntry)
Q_DECLARE_METATYPE(QNetworkInterface)
Q_DECLARE_METATYPE(QNetworkProxy)
Q_DECLARE_METATYPE(QSslCertificate)
Q_DECLARE_METATYPE(QSslCipher)
Q_DECLARE_METATYPE(QSslError)

Q_DECLARE_METATYPE(QList<QHostAddress>)                                                                                                          
Q_DECLARE_METATYPE(QList<QNetworkAddressEntry>)
Q_DECLARE_METATYPE(QList<QNetworkInterface>)
Q_DECLARE_METATYPE(QList<QNetworkProxy>)
Q_DECLARE_METATYPE(QList<QSslCertificate>)
Q_DECLARE_METATYPE(QList<QSslCipher>)
Q_DECLARE_METATYPE(QList<QSslError>)

namespace JSmoke {
    
DEF_CONTAINER_MARSHALLER(QListQHostAddress, QList<QHostAddress>)                                                                                                            
DEF_CONTAINER_MARSHALLER(QListQNetworkAddressEntry, QList<QNetworkAddressEntry>)
DEF_CONTAINER_MARSHALLER(QListQNetworkCookie, QList<QNetworkCookie>)
DEF_CONTAINER_MARSHALLER(QListQNetworkInterface, QList<QNetworkInterface>)
DEF_CONTAINER_MARSHALLER(QListQNetworkProxy, QList<QNetworkProxy>)
DEF_CONTAINER_MARSHALLER(QListQSslCertificate, QList<QSslCertificate>)
DEF_CONTAINER_MARSHALLER(QListQSslCipher, QList<QSslCipher>)
DEF_CONTAINER_MARSHALLER(QListQSslError, QList<QSslError>)

Marshall::TypeHandler QtNetworkHandlers[] = {
    { "QList<QHostAddress>", marshall_QListQHostAddress },                                                                                              
    { "QList<QHostAddress>&", marshall_QListQHostAddress },                                                                                                
    { "QList<QNetworkAddressEntry>&", marshall_QListQNetworkAddressEntry },                                                                                        
    { "QList<QNetworkCookie>", marshall_QListQNetworkCookie },                                                                                           
    { "QList<QNetworkCookie>&", marshall_QListQNetworkCookie },                                                                                              
    { "QList<QNetworkInterface>&", marshall_QListQNetworkInterface },                                                                                           
    { "QList<QNetworkProxy>&", marshall_QListQNetworkProxy },                                                                                               
    { "QList<QSslCertificate>&", marshall_QListQSslCertificate },
    { "QList<QSslCipher>&", marshall_QListQSslCipher },
    { "QList<QSslError>&", marshall_QListQSslError },
    
    { 0, 0 }
};

void registerQtNetworkTypes(QScriptEngine * engine) 
{   
    qScriptSmokeRegisterSequenceMetaType<QList<QHostAddress> >(engine);                                                                                                            
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkAddressEntry> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkCookie> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkInterface> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QNetworkProxy> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslCertificate> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslCipher> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslError> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
