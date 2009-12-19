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

DEF_CONTAINER_MARSHALLER(QListSopranoBackendSetting, QList<Soprano::BackendSetting>)
DEF_CONTAINER_MARSHALLER(QListSopranoBindingSet, QList<Soprano::BindingSet>)
DEF_CONTAINER_MARSHALLER(QListSopranoInferenceRule, QList<Soprano::Inference::Rule>)
DEF_CONTAINER_MARSHALLER(QListSopranoNode, QList<Soprano::Node>)
DEF_CONTAINER_MARSHALLER(QListSopranoStatement, QList<Soprano::Statement>)
DEF_CONTAINER_MARSHALLER(QListSopranoBackend, QList<Soprano::Backend*>)
DEF_CONTAINER_MARSHALLER(QListSopranoParser, QList<Soprano::Parser*>)
DEF_CONTAINER_MARSHALLER(QListSopranoSerializer, QList<Soprano::Serializer*>)

Marshall::TypeHandler SopranoHandlers[] = {
    { "QList<Soprano::BackendSetting>&", marshall_QListSopranoBackendSetting },
    { "QList<Soprano::BindingSet>", marshall_QListSopranoBindingSet },
    { "QList<Soprano::Inference::Rule>&", marshall_QListSopranoInferenceRule },
    { "QList<Soprano::Node>", marshall_QListSopranoNode },
    { "QList<Soprano::Node>&", marshall_QListSopranoNode },
    { "QList<Soprano::Statement>", marshall_QListSopranoStatement },
    { "QList<Soprano::Statement>&", marshall_QListSopranoStatement },
    { "QList<const Soprano::Backend*>", marshall_QListSopranoBackend },
    { "QList<const Soprano::Parser*>", marshall_QListSopranoParser },
    { "QList<const Soprano::Serializer*>", marshall_QListSopranoSerializer },
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
