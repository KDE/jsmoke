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

#include <smoke/qimageblitz_smoke.h>

namespace JSmoke {
extern Marshall::TypeHandler QImageBlitzHandlers[];
extern void registerQImageBlitzTypes(QScriptEngine * engine);  
}

void qtscript_initialize_jsmoke_qimageblitz_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_qimageblitz_Smoke();
        JSmoke::Module qimageblitz_module = { "qimageblitz", new JSmoke::Binding(qimageblitz_Smoke) };
        JSmoke::Global::modules[qimageblitz_Smoke] = qimageblitz_module;    
        JSmoke::Marshall::installHandlers(JSmoke::QImageBlitzHandlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    JSmoke::Global::initializeClasses(engine, qimageblitz_Smoke);
    JSmoke::registerQImageBlitzTypes(engine);
    
    return;
    
}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
