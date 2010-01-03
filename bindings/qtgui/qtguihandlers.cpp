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

#include <QtGui/QAbstractButton>
#include <QtGui/QAbstractTextDocumentLayout>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QColor>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QImageTextKeyLang>                                                                                                       
#include <QtGui/QKeySequence>
#include <QtGui/QListWidgetItem>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QPolygon>
#include <QtGui/QPrinterInfo>
#include <QtGui/QStandardItem>
#include <QtGui/QTableWidgetSelectionRange>
#include <QtGui/QTextBlock>
#include <QtGui/QTextEdit>
#include <QtGui/QTextFormat>
#include <QtGui/QTextLayout>
#include <QtGui/QTextLength>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QUndoStack>

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

Q_DECLARE_METATYPE2(QList<QPair<double,double> >)
Q_DECLARE_METATYPE2(QList<QPair<double,QPointF> >)
Q_DECLARE_METATYPE2(QVector<QPair<double,QColor> >)
Q_DECLARE_METATYPE(QAbstractButton*)
Q_DECLARE_METATYPE(QAbstractTextDocumentLayout::Selection)
Q_DECLARE_METATYPE(QAction*)
Q_DECLARE_METATYPE(QActionGroup*)
Q_DECLARE_METATYPE(QChar)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(QDockWidget*)
Q_DECLARE_METATYPE(QGraphicsView*)
Q_DECLARE_METATYPE(QGraphicsWidget*)
Q_DECLARE_METATYPE(QImageTextKeyLang)
Q_DECLARE_METATYPE(QKeySequence)
Q_DECLARE_METATYPE(QList<QAbstractButton*>)
Q_DECLARE_METATYPE(QList<QAction*>)
Q_DECLARE_METATYPE(QList<QActionGroup*>)
Q_DECLARE_METATYPE(QList<QChar>)
Q_DECLARE_METATYPE(QList<QColor>)
Q_DECLARE_METATYPE(QList<QDockWidget*>)
Q_DECLARE_METATYPE(QList<QGraphicsItem*>)
Q_DECLARE_METATYPE(QList<QGraphicsView*>)
Q_DECLARE_METATYPE(QList<QGraphicsWidget*>)
Q_DECLARE_METATYPE(QList<QImageTextKeyLang>)                                                                                                       
Q_DECLARE_METATYPE(QList<QKeySequence>)
Q_DECLARE_METATYPE(QList<QListWidgetItem*>)
Q_DECLARE_METATYPE(QList<QMdiSubWindow*>)
Q_DECLARE_METATYPE(QList<QModelIndex>)
Q_DECLARE_METATYPE(QList<QPersistentModelIndex>)
Q_DECLARE_METATYPE(QList<QPolygonF>)
Q_DECLARE_METATYPE(QList<QPrinterInfo>)
Q_DECLARE_METATYPE(QList<QStandardItem*>)
Q_DECLARE_METATYPE(QList<QTableWidgetItem*>)
Q_DECLARE_METATYPE(QList<QTableWidgetSelectionRange>)
Q_DECLARE_METATYPE(QList<QTextBlock>)
Q_DECLARE_METATYPE(QList<QTextEdit::ExtraSelection>)
Q_DECLARE_METATYPE(QList<QTextFrame*>)
Q_DECLARE_METATYPE(QList<QTextLayout::FormatRange>)
Q_DECLARE_METATYPE(QList<QTreeWidget*>)
Q_DECLARE_METATYPE(QList<QTreeWidgetItem*>)
Q_DECLARE_METATYPE(QList<QUndoStack*>)
Q_DECLARE_METATYPE(QList<QWidget*>)
Q_DECLARE_METATYPE(QListWidgetItem*)
Q_DECLARE_METATYPE(QLocale::Country)
Q_DECLARE_METATYPE(QMdiSubWindow*)
Q_DECLARE_METATYPE(QModelIndex)
Q_DECLARE_METATYPE(QPersistentModelIndex)
Q_DECLARE_METATYPE(QPolygonF)
Q_DECLARE_METATYPE(QPrinterInfo)
Q_DECLARE_METATYPE(QStandardItem*)
Q_DECLARE_METATYPE(QTableWidgetItem*)
Q_DECLARE_METATYPE(QTableWidgetSelectionRange)
Q_DECLARE_METATYPE(QTextBlock)
Q_DECLARE_METATYPE(QTextEdit::ExtraSelection)
Q_DECLARE_METATYPE(QTextFrame*)
Q_DECLARE_METATYPE(QTextLayout::FormatRange)
Q_DECLARE_METATYPE(QTreeWidget*)
Q_DECLARE_METATYPE(QTreeWidgetItem*)
Q_DECLARE_METATYPE(QUndoStack*)
Q_DECLARE_METATYPE(QVector<QAbstractTextDocumentLayout::Selection>)
Q_DECLARE_METATYPE(QVector<QColor>)
Q_DECLARE_METATYPE(QVector<QTextFormat>)
Q_DECLARE_METATYPE(QVector<QTextLength>)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER3(QListQPairDoubleDouble, QList<QPair<double,double> >)
DEF_CONTAINER_MARSHALLER3(QListQPairDoubleQPointF, QList<QPair<double,QPointF> >)
DEF_CONTAINER_MARSHALLER3(QVectorQPairDoubleQColor, QVector<QPair<double,QColor> >)
DEF_CONTAINER_MARSHALLER(QListQAbstractButton, QList<QAbstractButton*>)
DEF_CONTAINER_MARSHALLER(QListQActionGroup, QList<QActionGroup*>)
DEF_CONTAINER_MARSHALLER(QListQAction, QList<QAction*>)
DEF_CONTAINER_MARSHALLER(QListQChar, QList<QChar>)
DEF_CONTAINER_MARSHALLER(QListQColor, QList<QColor>)
DEF_CONTAINER_MARSHALLER(QListQDockWidget, QList<QDockWidget*>)
DEF_CONTAINER_MARSHALLER(QListQGraphicsItem, QList<QGraphicsItem*>)
DEF_CONTAINER_MARSHALLER(QListQGraphicsView, QList<QGraphicsView*>)
DEF_CONTAINER_MARSHALLER(QListQGraphicsWidget, QList<QGraphicsWidget*>)
DEF_CONTAINER_MARSHALLER(QListQImageTextKeyLang, QList<QImageTextKeyLang>)                                                                                                      
DEF_CONTAINER_MARSHALLER(QListQKeySequence, QList<QKeySequence>)
DEF_CONTAINER_MARSHALLER(QListQListWidgetItem, QList<QListWidgetItem*>)
DEF_CONTAINER_MARSHALLER(QListQMdiSubWindow, QList<QMdiSubWindow*>)
DEF_CONTAINER_MARSHALLER(QListQModelIndex, QList<QModelIndex>)
DEF_CONTAINER_MARSHALLER(QListQPersistentModelIndex, QList<QPersistentModelIndex>)
DEF_CONTAINER_MARSHALLER(QListQPolygonF, QList<QPolygonF>)
DEF_CONTAINER_MARSHALLER(QListQPrinterInfo, QList<QPrinterInfo>)
DEF_CONTAINER_MARSHALLER(QListQStandardItem, QList<QStandardItem*>)
DEF_CONTAINER_MARSHALLER(QListQTableWidgetItem, QList<QTableWidgetItem*>)
DEF_CONTAINER_MARSHALLER(QListQTableWidgetSelectionRange, QList<QTableWidgetSelectionRange>)
DEF_CONTAINER_MARSHALLER(QListQTextBlock, QList<QTextBlock>)
DEF_CONTAINER_MARSHALLER(QListQTextEditExtraSelection, QList<QTextEdit::ExtraSelection>)
DEF_CONTAINER_MARSHALLER(QListQTextFrame, QList<QTextFrame*>)
DEF_CONTAINER_MARSHALLER(QListQTextLayoutFormatRange, QList<QTextLayout::FormatRange>)
DEF_CONTAINER_MARSHALLER(QListQTreeWidgetItem, QList<QTreeWidgetItem*>)
DEF_CONTAINER_MARSHALLER(QListQTreeWidget, QList<QTreeWidget*>)
DEF_CONTAINER_MARSHALLER(QListQUndoStack, QList<QUndoStack*>)
DEF_CONTAINER_MARSHALLER(QListQWidget, QList<QWidget*>)
DEF_CONTAINER_MARSHALLER(QVectorQAbstractTextDocumentLayoutSelection, QVector<QAbstractTextDocumentLayout::Selection>)
DEF_CONTAINER_MARSHALLER(QVectorQColor, QVector<QColor>)
DEF_CONTAINER_MARSHALLER(QVectorQTextFormat, QVector<QTextFormat>)
DEF_CONTAINER_MARSHALLER(QVectorQTextLength, QVector<QTextLength>)

Marshall::TypeHandler QtGuiHandlers[] = {
    { "QList<QAbstractButton*>", marshall_QListQAbstractButton },
    { "QList<QAbstractButton*>&", marshall_QListQAbstractButton },
    { "QList<QActionGroup*>", marshall_QListQActionGroup },
    { "QList<QActionGroup*>&", marshall_QListQActionGroup },
    { "QList<QAction*>", marshall_QListQAction },
    { "QList<QAction*>&", marshall_QListQAction },
    { "QList<QChar>", marshall_QListQChar },
    { "QList<QColor>", marshall_QListQColor },
    { "QList<QColor>&", marshall_QListQColor },
    { "QList<QDockWidget*>", marshall_QListQDockWidget },
    { "QList<QDockWidget*>&", marshall_QListQDockWidget },
    { "QList<QFontDatabase::WritingSystem>", marshall_QListEnum },
    { "QList<QGraphicsItem*>", marshall_QListQGraphicsItem },
    { "QList<QGraphicsItem*>&", marshall_QListQGraphicsItem },
    { "QList<QGraphicsView*>", marshall_QListQGraphicsView },
    { "QList<QGraphicsView*>&", marshall_QListQGraphicsView },
    { "QList<QGraphicsWidget*>", marshall_QListQGraphicsWidget },
    { "QList<QGraphicsWidget*>&", marshall_QListQGraphicsWidget },
    { "QList<QImageTextKeyLang>&", marshall_QListQImageTextKeyLang },                                                                                           
    { "QList<QKeySequence>", marshall_QListQKeySequence },
    { "QList<QKeySequence>&", marshall_QListQKeySequence },
    { "QList<QKeySequence>&", marshall_QListQKeySequence },                                                                                            
    { "QList<QKeySequence>&", marshall_QListQKeySequence },                                                                                                
    { "QList<QListWidgetItem*>", marshall_QListQListWidgetItem },
    { "QList<QListWidgetItem*>&", marshall_QListQListWidgetItem },
    { "QList<QMdiSubWindow*>", marshall_QListQMdiSubWindow },
    { "QList<QMdiSubWindow*>&", marshall_QListQMdiSubWindow },
    { "QList<QModelIndex>", marshall_QListQModelIndex },
    { "QList<QModelIndex>&", marshall_QListQModelIndex },
    { "QList<QPair<double,double>>", marshall_QListQPairDoubleDouble },
    { "QList<QPair<double,QPointF>>", marshall_QListQPairDoubleQPointF },
    { "QList<QPersistentModelIndex>", marshall_QListQPersistentModelIndex },
    { "QList<QPolygonF>&", marshall_QListQPolygonF },
    { "QList<QPrinterInfo>&", marshall_QListQPrinterInfo },
    { "QList<QPrinter::PageSize>", marshall_QListEnum },
    { "QList<QStandardItem*>", marshall_QListQStandardItem },
    { "QList<QStandardItem*>&", marshall_QListQStandardItem },
    { "QList<QTableWidgetItem*>", marshall_QListQTableWidgetItem },
    { "QList<QTableWidgetItem*>&", marshall_QListQTableWidgetItem },
    { "QList<QTableWidgetSelectionRange>&", marshall_QListQTableWidgetSelectionRange },
    { "QList<QTextBlock>&", marshall_QListQTextBlock },
    { "QList<QTextEdit::ExtraSelection>", marshall_QListQTextEditExtraSelection },
    { "QList<QTextEdit::ExtraSelection>&", marshall_QListQTextEditExtraSelection },
    { "QList<QTextFrame*>", marshall_QListQTextFrame },
    { "QList<QTextFrame*>&", marshall_QListQTextFrame },
    { "QList<QTextLayout::FormatRange>", marshall_QListQTextLayoutFormatRange },
    { "QList<QTextLayout::FormatRange>&", marshall_QListQTextLayoutFormatRange },
    { "QList<QTextOption::Tab>", marshall_QListEnum },
    { "QList<QTextOption::Tab>&", marshall_QListEnum },
    { "QList<QTreeWidgetItem*>", marshall_QListQTreeWidgetItem },
    { "QList<QTreeWidgetItem*>&", marshall_QListQTreeWidgetItem },
    { "QList<QTreeWidget*>", marshall_QListQTreeWidget },
    { "QList<QTreeWidget*>&", marshall_QListQTreeWidget },
    { "QList<QUndoStack*>", marshall_QListQUndoStack },
    { "QList<QUndoStack*>&", marshall_QListQUndoStack },
    { "QList<QWidget*>", marshall_QListQWidget },
    { "QList<QWidget*>&", marshall_QListQWidget },
    { "QList<QWidget*>&", marshall_QListQWidget }, 
    { "QList<QWizard::WizardButton>&", marshall_QListEnum },     
    { "QListView::Flow", marshall_QListEnum },
    { "QListView::LayoutMode", marshall_QListEnum },
    { "QListView::Movement", marshall_QListEnum },
    { "QListView::ResizeMode", marshall_QListEnum },
    { "QListView::ViewMode", marshall_QListEnum },
    { "QListWidgetItem::ItemType", marshall_QListEnum },
    { "QVector<QAbstractTextDocumentLayout::Selection>", marshall_QVectorQAbstractTextDocumentLayoutSelection },
    { "QVector<QAbstractTextDocumentLayout::Selection>&", marshall_QVectorQAbstractTextDocumentLayoutSelection },
    { "QVector<QColor>", marshall_QVectorQColor },
    { "QVector<QPair<double,QColor>>", marshall_QVectorQPairDoubleQColor },
    { "QVector<QPair<double,QColor>>&", marshall_QVectorQPairDoubleQColor },
    { "QVector<QTextFormat>", marshall_QVectorQTextFormat },
    { "QVector<QTextLength>", marshall_QVectorQTextLength },
    { "QVector<QTextLength>&", marshall_QVectorQTextLength },

    { 0, 0 }
};

void registerQtGuiTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<double,double> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QList<QPair<double,QPointF> > >(engine);
    qScriptSmokeRegisterPairSequenceMetaType<QVector<QPair<double,QColor> > >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QAbstractButton*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QAction*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QActionGroup*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QDockWidget*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QGraphicsItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QGraphicsView*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QGraphicsWidget*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QListWidgetItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QMdiSubWindow*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QStandardItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QTableWidgetItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QTextFrame*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QTreeWidget*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QTreeWidgetItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QUndoStack*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QWidget*> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QChar> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QColor> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QImageTextKeyLang> >(engine);                                                                                                       
    qScriptSmokeRegisterSequenceMetaType<QList<QKeySequence> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QModelIndex> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QPersistentModelIndex> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QPolygonF> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QPrinterInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTableWidgetSelectionRange> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTextBlock> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTextEdit::ExtraSelection> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QTextLayout::FormatRange> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QAbstractTextDocumentLayout::Selection> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QColor> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QTextFormat> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QVector<QTextLength> >(engine); 

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
