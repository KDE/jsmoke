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

Marshall::TypeHandler KDEUiHandlers[] = {
//    { "QList<QPair<KGuiItem,KGuiItem>>", marshall_Container<QList<QPairKGuiItemKGuiItem> > },
    { "QList<KAction*>&", marshall_Container<QList<KAction*> > },
    { "QList<KActionCollection*>", marshall_Container<QList<KActionCollection*> > },
    { "QList<KActionCollection*>&", marshall_Container<QList<KActionCollection*> > },
    { "QList<KConfigDialogManager*>", marshall_Container<QList<KConfigDialogManager*> > },
    { "QList<KIconTheme*>&", marshall_Container<QList<KIconTheme*> > },
    { "QList<KMainWindow*>", marshall_Container<QList<KMainWindow*> > },
    { "QList<KPlotObject*>", marshall_Container<QList<KPlotObject*> > },
    { "QList<KPlotObject*>&", marshall_Container<QList<KPlotObject*> > },
    { "QList<KPlotPoint*>", marshall_Container<QList<KPlotPoint*> > },
    { "QList<KSessionManager*>&", marshall_Container<QList<KSessionManager*> > },
    { "QList<KStandardAction::StandardAction>", marshall_Container<QList<KStandardAction::StandardAction> > },
    { "QList<KToolBar*>", marshall_Container<QList<KToolBar*> > },
    { "QList<KXMLGUIClient*>", marshall_Container<QList<KXMLGUIClient*> > },
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
