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

#include <smoke/qtsvg_smoke.h>

namespace QtScriptSmoke {
extern Marshall::TypeHandler QtSvgHandlers[];
extern void registerQtSvgTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_qt_svg_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_qtsvg_Smoke();
        QtScriptSmoke::Module qtsvg_module = { "qtsvg", new QtScriptSmoke::Binding(qtsvg_Smoke) };
        QtScriptSmoke::Global::modules[qtsvg_Smoke] = qtsvg_module;    
        QtScriptSmoke::Marshall::installHandlers(QtScriptSmoke::QtSvgHandlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    QtScriptSmoke::Global::initializeClasses(engine, qtsvg_Smoke);
    QtScriptSmoke::registerQtSvgTypes(engine);
    
    return;
    
}
