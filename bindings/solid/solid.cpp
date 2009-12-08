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

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

#include <global.h>
#include <marshall.h>

#include <smoke/solid_smoke.h>

namespace QtScriptSmoke {
extern Marshall::TypeHandler SolidHandlers[];
extern void registerSolidTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_solid_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_solid_Smoke();
        QtScriptSmoke::Module solid_module = { "solid", new QtScriptSmoke::Binding(solid_Smoke) };
        QtScriptSmoke::Global::modules[solid_Smoke] = solid_module;    
        QtScriptSmoke::Marshall::installHandlers(QtScriptSmoke::SolidHandlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    QtScriptSmoke::Global::initializeClasses(engine, solid_Smoke);
    QtScriptSmoke::registerSolidTypes(engine);
    
    return;
    
}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
