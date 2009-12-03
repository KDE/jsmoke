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

#include <smoke/qtuitools_smoke.h>

namespace QtScriptSmoke {
extern Marshall::TypeHandler QtUiToolsHandlers[];
extern void registerQtUiToolsTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_qt_uitools_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_qtuitools_Smoke();
        QtScriptSmoke::Module qtuitools_module = { "qtuitools", new QtScriptSmoke::Binding(qtuitools_Smoke) };
        QtScriptSmoke::Global::modules[qtuitools_Smoke] = qtuitools_module;    
        QtScriptSmoke::Marshall::installHandlers(QtScriptSmoke::QtUiToolsHandlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    QtScriptSmoke::Global::initializeClasses(engine, qtuitools_Smoke);
    QtScriptSmoke::registerQtUiToolsTypes(engine);
    
    return;
    
}
