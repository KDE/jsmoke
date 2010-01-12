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

#include <ktexteditor/commandinterface.h>
#include <ktexteditor/containerinterface.h>
#include <ktexteditor/document.h>
#include <ktexteditor/range.h>
#include <ktexteditor/smartrange.h>
#include <ktexteditor/smartrangenotifier.h>
#include <ktexteditor/smartrangewatcher.h>
#include <ktexteditor/view.h>

Q_DECLARE_METATYPE(KTextEditor::Command*)
Q_DECLARE_METATYPE(KTextEditor::Range)
Q_DECLARE_METATYPE(KTextEditor::SmartRange*)
Q_DECLARE_METATYPE(KTextEditor::SmartRangeNotifier*)
Q_DECLARE_METATYPE(KTextEditor::SmartRangeWatcher*)
Q_DECLARE_METATYPE(KTextEditor::View*)
Q_DECLARE_METATYPE(QList<KTextEditor::Command*>)
Q_DECLARE_METATYPE(QList<KTextEditor::Document*>)
Q_DECLARE_METATYPE(QList<KTextEditor::SmartRange*>)
Q_DECLARE_METATYPE(QList<KTextEditor::SmartRangeNotifier*>)
Q_DECLARE_METATYPE(QList<KTextEditor::SmartRangeWatcher*>)
Q_DECLARE_METATYPE(QList<KTextEditor::View*>)
Q_DECLARE_METATYPE(QVector<KTextEditor::Range>)

namespace JSmoke {

Marshall::TypeHandler KTextEditorHandlers[] = {
    { "QList<KTextEditor::Command*>", marshall_Container<QList<KTextEditor::Command*> > },
    { "QList<KTextEditor::Document*>&", marshall_Container<QList<KTextEditor::Document*> > },
    { "QList<KTextEditor::SmartRange*>", marshall_Container<QList<KTextEditor::SmartRange*> > },
    { "QList<KTextEditor::SmartRange*>&", marshall_Container<QList<KTextEditor::SmartRange*> > },
    { "QList<KTextEditor::SmartRangeNotifier*>", marshall_Container<QList<KTextEditor::SmartRangeNotifier*> > },
    { "QList<KTextEditor::SmartRangeWatcher*>&", marshall_Container<QList<KTextEditor::SmartRangeWatcher*> > },
    { "QList<KTextEditor::View*>&", marshall_Container<QList<KTextEditor::View*> > },
    { "QVector<KTextEditor::Range>", marshall_Container<QVector<KTextEditor::Range> > },
    { 0, 0 }
};

void registerKTextEditorTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QVector<KTextEditor::Range> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::Command*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::Document*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::SmartRange*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::SmartRangeNotifier*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::SmartRangeWatcher*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KTextEditor::View*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
