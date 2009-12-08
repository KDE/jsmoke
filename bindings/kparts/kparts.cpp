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

#include <smoke/kparts_smoke.h>

namespace QtScriptSmoke {
extern Marshall::TypeHandler KPartsHandlers[];
extern void registerKPartsTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_kparts_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_kparts_Smoke();
        QtScriptSmoke::Module kparts_module = { "kparts", new QtScriptSmoke::Binding(kparts_Smoke) };
        QtScriptSmoke::Global::modules[kparts_Smoke] = kparts_module;    
        QtScriptSmoke::Marshall::installHandlers(QtScriptSmoke::KPartsHandlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    QtScriptSmoke::Global::initializeClasses(engine, kparts_Smoke);
    QtScriptSmoke::registerKPartsTypes(engine);
    
    return;
    
}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
