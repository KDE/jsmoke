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

#include <attica/provider.h>
#include <attica/providermanager.h>

Q_DECLARE_METATYPE(Attica::Provider)
Q_DECLARE_METATYPE(QList<Attica::Provider>)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER(QListAtticaProvider, QList<Attica::Provider>)

Marshall::TypeHandler AtticaHandlers[] = {
    { "QList<Attica::Provider>", marshall_QListAtticaProvider },
    { 0, 0 }
};

void registerAtticaTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<Attica::Provider> >(engine);
 
    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
