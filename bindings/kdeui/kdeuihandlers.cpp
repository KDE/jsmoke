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

#include <kactioncollection.h>
#include <kaction.h>
#include <kconfigdialogmanager.h>
#include <kguiitem.h>
#include <kmainwindow.h>
#include <kplotobject.h>
#include <ksessionmanager.h>
#include <ktoolbar.h>

// Q_DECLARE_METATYPE2(QList<QPair<KGuiItem,KGuiItem> >)
// Q_DECLARE_METATYPE(KGuiItem)
Q_DECLARE_METATYPE(KAction*)
Q_DECLARE_METATYPE(QList<KAction*>)
Q_DECLARE_METATYPE(KActionCollection*)
Q_DECLARE_METATYPE(KConfigDialogManager*)
Q_DECLARE_METATYPE(KIconTheme*)
Q_DECLARE_METATYPE(KMainWindow*)
Q_DECLARE_METATYPE(KPlotObject*)
Q_DECLARE_METATYPE(KPlotPoint*)
Q_DECLARE_METATYPE(KSessionManager*)
Q_DECLARE_METATYPE(KStandardAction::StandardAction)
Q_DECLARE_METATYPE(KToolBar*)
Q_DECLARE_METATYPE(KXMLGUIClient*)
Q_DECLARE_METATYPE(QList<KActionCollection*>)
Q_DECLARE_METATYPE(QList<KConfigDialogManager*>)
Q_DECLARE_METATYPE(QList<KIconTheme*>)
Q_DECLARE_METATYPE(QList<KMainWindow*>)
Q_DECLARE_METATYPE(QList<KPlotObject*>)
Q_DECLARE_METATYPE(QList<KPlotPoint*>)
Q_DECLARE_METATYPE(QList<KSessionManager*>)
Q_DECLARE_METATYPE(QList<KStandardAction::StandardAction>)
Q_DECLARE_METATYPE(QList<KToolBar*>)
Q_DECLARE_METATYPE(QList<KXMLGUIClient*>)

namespace JSmoke {

// DEF_CONTAINER_MARSHALLER3(QListQPairKGuiItemKGuiItem, QList<QPair<KGuiItem,KGuiItem> >)
DEF_CONTAINER_MARSHALLER(QListKAction, QList<KAction*>)
DEF_CONTAINER_MARSHALLER(QListKActionCollection, QList<KActionCollection*>)
DEF_CONTAINER_MARSHALLER(QListKConfigDialogManager, QList<KConfigDialogManager*>)
DEF_CONTAINER_MARSHALLER(QListKIconTheme, QList<KIconTheme*>)
DEF_CONTAINER_MARSHALLER(QListKMainWindow, QList<KMainWindow*>)
DEF_CONTAINER_MARSHALLER(QListKPlotObject, QList<KPlotObject*>)
DEF_CONTAINER_MARSHALLER(QListKPlotPoint, QList<KPlotPoint*>)
DEF_CONTAINER_MARSHALLER(QListKSessionManager, QList<KSessionManager*>)
DEF_CONTAINER_MARSHALLER(QListKStandardActionStandardAction, QList<KStandardAction::StandardAction>)
DEF_CONTAINER_MARSHALLER(QListKToolBar, QList<KToolBar*>)
DEF_CONTAINER_MARSHALLER(QListKXMLGUIClient, QList<KXMLGUIClient*>)

Marshall::TypeHandler KDEUiHandlers[] = {
//    { "QList<QPair<KGuiItem,KGuiItem>>", marshall_QListQPairKGuiItemKGuiItem },
    { "QList<KAction*>&", marshall_QListKAction },
    { "QList<KActionCollection*>", marshall_QListKActionCollection },
    { "QList<KActionCollection*>&", marshall_QListKActionCollection },
    { "QList<KConfigDialogManager*>", marshall_QListKConfigDialogManager },
    { "QList<KIconTheme*>&", marshall_QListKIconTheme },
    { "QList<KMainWindow*>", marshall_QListKMainWindow },
    { "QList<KPlotObject*>", marshall_QListKPlotObject },
    { "QList<KPlotObject*>&", marshall_QListKPlotObject },
    { "QList<KPlotPoint*>", marshall_QListKPlotPoint },
    { "QList<KSessionManager*>&", marshall_QListKSessionManager },
    { "QList<KStandardAction::StandardAction>", marshall_QListKStandardActionStandardAction },
    { "QList<KToolBar*>", marshall_QListKToolBar },
    { "QList<KXMLGUIClient*>", marshall_QListKXMLGUIClient },
    { 0, 0 }
};

void registerKDEUiTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<KStandardAction::StandardAction> >(engine);
//    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<KGuiItem,KGuiItem> > >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KAction*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KActionCollection*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KConfigDialogManager*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KIconTheme*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KMainWindow*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KPlotObject*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KPlotPoint*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KSessionManager*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KToolBar*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KXMLGUIClient*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
