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

#include <soprano/backend.h>
#include <soprano/bindingset.h>
#include <soprano/dbusnodeiterator.h>
#include <soprano/dbusstatementiterator.h>
#include <soprano/inferencerule.h>
#include <soprano/node.h>
#include <soprano/nodeiterator.h>
#include <soprano/nodepattern.h>
#include <soprano/parser.h>
#include <soprano/serializer.h>
#include <soprano/simplenodeiterator.h>
#include <soprano/simplestatementiterator.h>
#include <soprano/statement.h>
#include <soprano/statementiterator.h>
#include <soprano/statementpattern.h>

Q_DECLARE_METATYPE(QList<Soprano::BackendSetting>)
Q_DECLARE_METATYPE(QList<Soprano::BindingSet>)
Q_DECLARE_METATYPE(QList<Soprano::Inference::Rule>)
Q_DECLARE_METATYPE(QList<Soprano::Node>)
Q_DECLARE_METATYPE(QList<Soprano::Statement>)
Q_DECLARE_METATYPE(QList<Soprano::Backend*>)
Q_DECLARE_METATYPE(QList<Soprano::Parser*>)
Q_DECLARE_METATYPE(QList<Soprano::Serializer*>)
Q_DECLARE_METATYPE(Soprano::BackendSetting)
Q_DECLARE_METATYPE(Soprano::BindingSet)
Q_DECLARE_METATYPE(Soprano::Inference::Rule)
Q_DECLARE_METATYPE(Soprano::Node)
Q_DECLARE_METATYPE(Soprano::Statement)
Q_DECLARE_METATYPE(Soprano::Backend*)
Q_DECLARE_METATYPE(Soprano::Parser*)
Q_DECLARE_METATYPE(Soprano::Serializer*)

namespace JSmoke {

Marshall::TypeHandler SopranoHandlers[] = {
    { "QList<Soprano::BackendSetting>&", marshall_Container<QList<Soprano::BackendSetting> > },
    { "QList<Soprano::BindingSet>", marshall_Container<QList<Soprano::BindingSet> > },
    { "QList<Soprano::Inference::Rule>&", marshall_Container<QList<Soprano::Inference::Rule> > },
    { "QList<Soprano::Node>", marshall_Container<QList<Soprano::Node> > },
    { "QList<Soprano::Node>&", marshall_Container<QList<Soprano::Node> > },
    { "QList<Soprano::Statement>", marshall_Container<QList<Soprano::Statement> > },
    { "QList<Soprano::Statement>&", marshall_Container<QList<Soprano::Statement> > },
    { "QList<const Soprano::Backend*>", marshall_Container<QList<Soprano::Backend*> > },
    { "QList<const Soprano::Parser*>", marshall_Container<QList<Soprano::Parser*> > },
    { "QList<const Soprano::Serializer*>", marshall_Container<QList<Soprano::Serializer*> > },
    { 0, 0 }
};

void registerSopranoTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::BackendSetting> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::BindingSet> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::Inference::Rule> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::Node> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::Statement> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<Soprano::Backend*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Soprano::Parser*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Soprano::Serializer*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
