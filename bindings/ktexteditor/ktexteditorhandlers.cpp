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

DEF_CONTAINER_MARSHALLER(QListKTextEditorCommand, QList<KTextEditor::Command*>)
DEF_CONTAINER_MARSHALLER(QListKTextEditorDocument, QList<KTextEditor::Document*>)
DEF_CONTAINER_MARSHALLER(QListKTextEditorSmartRange, QList<KTextEditor::SmartRange*>)
DEF_CONTAINER_MARSHALLER(QListKTextEditorSmartRangeNotifier, QList<KTextEditor::SmartRangeNotifier*>)
DEF_CONTAINER_MARSHALLER(QListKTextEditorSmartRangeWatcher, QList<KTextEditor::SmartRangeWatcher*>)
DEF_CONTAINER_MARSHALLER(QListKTextEditorView, QList<KTextEditor::View*>)
DEF_CONTAINER_MARSHALLER(QVectorKTextEditorRange, QVector<KTextEditor::Range>)

Marshall::TypeHandler KTextEditorHandlers[] = {
    { "QList<KTextEditor::Command*>", marshall_QListKTextEditorCommand },
    { "QList<KTextEditor::Document*>&", marshall_QListKTextEditorDocument },
    { "QList<KTextEditor::SmartRange*>", marshall_QListKTextEditorSmartRange },
    { "QList<KTextEditor::SmartRange*>&", marshall_QListKTextEditorSmartRange },
    { "QList<KTextEditor::SmartRangeNotifier*>", marshall_QListKTextEditorSmartRangeNotifier },
    { "QList<KTextEditor::SmartRangeWatcher*>&", marshall_QListKTextEditorSmartRangeWatcher },
    { "QList<KTextEditor::View*>&", marshall_QListKTextEditorView },
    { "QVector<KTextEditor::Range>", marshall_QVectorKTextEditorRange },
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
