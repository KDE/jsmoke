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

#include <kparts/event.h>
#include <kparts/mainwindow.h>
#include <kparts/part.h>
#include <kparts/partmanager.h>
#include <kparts/plugin.h>

Q_DECLARE_METATYPE(KParts::Part*)
Q_DECLARE_METATYPE(KParts::Plugin*)
Q_DECLARE_METATYPE(KParts::Plugin::PluginInfo)
Q_DECLARE_METATYPE(KParts::ReadOnlyPart*)
Q_DECLARE_METATYPE(QList<KParts::Part*>)
Q_DECLARE_METATYPE(QList<KParts::Plugin*>)
Q_DECLARE_METATYPE(QList<KParts::Plugin::PluginInfo>)
Q_DECLARE_METATYPE(QList<KParts::ReadOnlyPart*>)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER(QListKPartsPart, QList<KParts::Part*>)
DEF_CONTAINER_MARSHALLER(QListKPartsPlugin, QList<KParts::Plugin*>)
DEF_CONTAINER_MARSHALLER(QListKPartsPluginPluginInfo, QList<KParts::Plugin::PluginInfo>)
DEF_CONTAINER_MARSHALLER(QListKPartsReadOnlyPart, QList<KParts::ReadOnlyPart*>)

Marshall::TypeHandler KPartsHandlers[] = {
    { "QList<KParts::Part*>", marshall_QListKPartsPart },
    { "QList<KParts::Plugin*>", marshall_QListKPartsPlugin },
    { "QList<KParts::Plugin::PluginInfo>", marshall_QListKPartsPluginPluginInfo },
    { "QList<KParts::Plugin::PluginInfo>&", marshall_QListKPartsPluginPluginInfo },
    { "QList<KParts::ReadOnlyPart*>", marshall_QListKPartsReadOnlyPart },
    { 0, 0 }
};

void registerKPartsTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<KParts::Plugin::PluginInfo> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<KParts::Part*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KParts::Plugin*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KParts::ReadOnlyPart*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
