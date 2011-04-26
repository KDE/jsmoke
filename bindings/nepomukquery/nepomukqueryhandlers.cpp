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

#include <nepomuk/query.h>
#include <nepomuk/result.h>
#include <nepomuk/term.h>

// Q_DECLARE_METATYPE(Nepomuk::Query::Query::RequestProperty)
Q_DECLARE_METATYPE(Nepomuk::Query::Result)
Q_DECLARE_METATYPE(Nepomuk::Query::Term)
Q_DECLARE_METATYPE(Nepomuk::Types::Property)
// Q_DECLARE_METATYPE(QList<Nepomuk::Query::Query::RequestProperty>)
Q_DECLARE_METATYPE(QList<Nepomuk::Query::Result>)
Q_DECLARE_METATYPE(QList<Nepomuk::Query::Term>)
Q_DECLARE_METATYPE(QList<Nepomuk::Types::Property>)

namespace JSmoke {

Marshall::TypeHandler NepomukQueryHandlers[] = {
//    { "QList<Nepomuk::Query::Query::RequestProperty>", marshall_Container<QList<NepomukQueryQueryRequestProperty> > },
//    { "QList<Nepomuk::Query::Query::RequestProperty>&", marshall_Container<QList<NepomukQueryQueryRequestProperty> > },
    { "QList<Nepomuk::Query::Result>&", marshall_Container<QList<Nepomuk::Query::Result> > },
    { "QList<Nepomuk::Query::Term>", marshall_Container<QList<Nepomuk::Query::Term> > },
    { "QList<Nepomuk::Query::Term>&", marshall_Container<QList<Nepomuk::Query::Term> > },
    { "QList<Nepomuk::Types::Property>", marshall_Container<QList<Nepomuk::Types::Property> > },
    { 0, 0 }
};

void registerNepomukQueryTypes(QScriptEngine * engine)
{
//    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Query::Query::RequestProperty> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Query::Result> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Query::Term> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Nepomuk::Types::Property> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
