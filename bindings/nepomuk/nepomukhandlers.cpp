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

#include <nepomuk/kmetadatatagcloud.h>
#include <nepomuk/kmetadatatagwidget.h>
#include <nepomuk/property.h>
#include <nepomuk/resource.h>
#include <nepomuk/resourcemanager.h>
#include <nepomuk/resourceterm.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/tag.h>
#include <nepomuk/thing.h>
#include <nepomuk/variant.h>
#include <soprano/node.h>
// #include <soprano/nodeiterator.h>
#include <soprano/nodepattern.h>
// #include <soprano/simplenodeiterator.h>
// #include <soprano/simplestatementiterator.h>
#include <soprano/statement.h>
// #include <soprano/statementiterator.h>
#include <soprano/statementpattern.h>

// Q_DECLARE_METATYPE(Nepomuk::Resource)
Q_DECLARE_METATYPE(Nepomuk::Tag)
Q_DECLARE_METATYPE(Nepomuk::Types::Class)
Q_DECLARE_METATYPE(Nepomuk::Types::Property)
Q_DECLARE_METATYPE(Nepomuk::Variant)
Q_DECLARE_METATYPE(QList<Nepomuk::Tag>)
Q_DECLARE_METATYPE(QList<Nepomuk::Types::Class>)
Q_DECLARE_METATYPE(QList<Nepomuk::Types::Property>)
Q_DECLARE_METATYPE(QList<Nepomuk::Variant>)
Q_DECLARE_METATYPE(QList<Soprano::Node>)
Q_DECLARE_METATYPE(QList<Soprano::Statement>)
Q_DECLARE_METATYPE(QList<Nepomuk::Class*>)
Q_DECLARE_METATYPE(QList<Nepomuk::Property*>)
Q_DECLARE_METATYPE(Soprano::Node)
Q_DECLARE_METATYPE(Soprano::Statement)
Q_DECLARE_METATYPE(Nepomuk::Class*)
Q_DECLARE_METATYPE(Nepomuk::Property*)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER(QListNepomukResource, QList<Nepomuk::Resource>)
DEF_CONTAINER_MARSHALLER(QListNepomukTag, QList<Nepomuk::Tag>)
DEF_CONTAINER_MARSHALLER(QListNepomukTypesClass, QList<Nepomuk::Types::Class>)
DEF_CONTAINER_MARSHALLER(QListNepomukTypesProperty, QList<Nepomuk::Types::Property>)
DEF_CONTAINER_MARSHALLER(QListNepomukVariant, QList<Nepomuk::Variant>)
DEF_CONTAINER_MARSHALLER(QListSopranoNode, QList<Soprano::Node>)
DEF_CONTAINER_MARSHALLER(QListSopranoStatement, QList<Soprano::Statement>)
DEF_CONTAINER_MARSHALLER(QListNepomukClass, QList<Nepomuk::Class*>)
DEF_CONTAINER_MARSHALLER(QListNepomukProperty, QList<Nepomuk::Property*>)

Marshall::TypeHandler NepomukHandlers[] = {
    { "QList<Nepomuk::Resource>", marshall_QListNepomukResource },
    { "QList<Nepomuk::Resource>&", marshall_QListNepomukResource },
    { "QList<Nepomuk::Tag>", marshall_QListNepomukTag },
    { "QList<Nepomuk::Tag>&", marshall_QListNepomukTag },
    { "QList<Nepomuk::Types::Class>", marshall_QListNepomukTypesClass },
    { "QList<Nepomuk::Types::Property>", marshall_QListNepomukTypesProperty },
    { "QList<Nepomuk::Variant>", marshall_QListNepomukVariant },
    { "QList<Nepomuk::Variant>&", marshall_QListNepomukVariant },
    { "QList<Soprano::Node>", marshall_QListSopranoNode },
    { "QList<Soprano::Statement>", marshall_QListSopranoStatement },
    { "QList<const Nepomuk::Class*>", marshall_QListNepomukClass },
    { "QList<const Nepomuk::Property*>", marshall_QListNepomukProperty },
    { 0, 0 }
};

void registerNepomukTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Resource> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Tag> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Types::Class> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Types::Property> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Variant> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::Node> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::Statement> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<Nepomuk::Class*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Nepomuk::Property*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
