/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

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

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

#include <kbookmarkmenu.h>
#include <kdatatool.h>
#include <kfileitemdelegate.h>
#include <kio/accessmanager.h>
#include <kio/udsentry.h>
#include <kio/copyjob.h>

Q_DECLARE_METATYPE(KBookmarkMenu*)
Q_DECLARE_METATYPE(KDataToolInfo)
Q_DECLARE_METATYPE(KFileItemDelegate::Information)
Q_DECLARE_METATYPE(KIO::CopyInfo)
Q_DECLARE_METATYPE(KIO::UDSEntry)
Q_DECLARE_METATYPE(KServiceAction)
// Q_DECLARE_METATYPE(KSharedPtr<KService>)
Q_DECLARE_METATYPE(QList<KBookmarkMenu*>)
Q_DECLARE_METATYPE(QList<KDataToolInfo>)
Q_DECLARE_METATYPE(QList<KFileItemDelegate::Information>)
Q_DECLARE_METATYPE(QList<KIO::CopyInfo>)
Q_DECLARE_METATYPE(QList<KIO::UDSEntry>)
Q_DECLARE_METATYPE(QList<KServiceAction>)
//Q_DECLARE_METATYPE(QList<KSharedPtr<KService>>)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER(QListKBookmarkMenu, QList<KBookmarkMenu*>)
DEF_CONTAINER_MARSHALLER(QListKDataToolInfo, QList<KDataToolInfo>)
DEF_CONTAINER_MARSHALLER(QListKFileItemDelegateInformation, QList<KFileItemDelegate::Information>)
DEF_CONTAINER_MARSHALLER(QListKIOCopyInfo, QList<KIO::CopyInfo>)
DEF_CONTAINER_MARSHALLER(QListKIOUDSEntry, QList<KIO::UDSEntry>)
DEF_CONTAINER_MARSHALLER(QListKServiceAction, QList<KServiceAction>)
// DEF_CONTAINER_MARSHALLER(QListKSharedPtr<KService>, QList<KSharedPtr<KService>>)

Marshall::TypeHandler KIOHandlers[] = {
    { "QList<KBookmarkMenu*>&", marshall_QListKBookmarkMenu },
    { "QList<KDataToolInfo>&", marshall_QListKDataToolInfo },
    { "QList<KFileItemDelegate::Information>", marshall_QListKFileItemDelegateInformation },
    { "QList<KFileItemDelegate::Information>&", marshall_QListKFileItemDelegateInformation },
    { "QList<KIO::CopyInfo>&", marshall_QListKIOCopyInfo },
    { "QList<KIO::UDSEntry>&", marshall_QListKIOUDSEntry },
    { "QList<KServiceAction>", marshall_QListKServiceAction },
//    { "QList<KSharedPtr<KService>>", marshall_QListKSharedPtr<KService> },
    { 0, 0 }
};

void registerKIOTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<KDataToolInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KFileItemDelegate::Information> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KIO::CopyInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KIO::UDSEntry> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KServiceAction> >(engine);
    
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KBookmarkMenu*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
