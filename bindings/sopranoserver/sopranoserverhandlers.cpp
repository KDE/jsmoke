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

Q_DECLARE_METATYPE(QList<Soprano::BackendSetting>)
Q_DECLARE_METATYPE(Soprano::BackendSetting)

namespace JSmoke {

Marshall::TypeHandler SopranoServerHandlers[] = {
    { "QList<Soprano::BackendSetting>", marshall_Container<QList<Soprano::BackendSetting> > },
    { "QList<Soprano::BackendSetting>&", marshall_Container<QList<Soprano::BackendSetting> > },
    { 0, 0 }
};

void registerSopranoServerTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<Soprano::BackendSetting> >(engine);


    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
