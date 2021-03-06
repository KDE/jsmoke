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
#include "qscriptmetatype.h"

#include <kbookmarkmenu.h>
#include <kdatatool.h>
#include <kfileitem.h>
#include <kfileitemdelegate.h>
#include <kio/accessmanager.h>
#include <kio/udsentry.h>
#include <kio/copyjob.h>

Q_DECLARE_METATYPE2(QList<QPair<KFileItem,KFileItem> >)
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

Marshall::TypeHandler KIOHandlers[] = {
    { "QList<QPair<KFileItem,KFileItem>>", marshall_Container<QList<QPair<KFileItem,KFileItem> > > },
    { "QList<KBookmarkMenu*>&", marshall_Container<QList<KBookmarkMenu*> > },
    { "QList<KDataToolInfo>&", marshall_Container<QList<KDataToolInfo> > },
    { "QList<KFileItemDelegate::Information>", marshall_Container<QList<KFileItemDelegate::Information> > },
    { "QList<KFileItemDelegate::Information>&", marshall_Container<QList<KFileItemDelegate::Information> > },
    { "QList<KIO::CopyInfo>&", marshall_Container<QList<KIO::CopyInfo> > },
    { "QList<KIO::UDSEntry>&", marshall_Container<QList<KIO::UDSEntry> > },
    { "QList<KServiceAction>", marshall_Container<QList<KServiceAction> > },
//    { "QList<KSharedPtr<KService>>", marshall_Container<QList<KSharedPtr> ><KService> },
    { 0, 0 }
};

void registerKIOTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<KDataToolInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KFileItemDelegate::Information> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KIO::CopyInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KIO::UDSEntry> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KServiceAction> >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<KFileItem,KFileItem> > >(engine);
    
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KBookmarkMenu*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
