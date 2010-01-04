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

#include <QtGui/QGraphicsItem>

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

#include <global.h>
#include <marshall.h>

#include <smoke/qtgui_smoke.h>

static void
qgraphicsitemTypeResolver(JSmoke::Object::Instance * instance)
{
    Smoke * smoke = instance->classId.smoke;    
    QGraphicsItem * item = static_cast<QGraphicsItem*>( smoke->cast(    instance->value, 
                                                                        instance->classId,
                                                                        JSmoke::Global::QGraphicsItemClassId ) );
    switch (item->type()) {
    case 1:
        instance->classId = smoke->findClass("QGraphicsItem");
        break;
    case 2:
        instance->classId = smoke->findClass("QGraphicsPathItem");
        break;
    case 3:
        instance->classId = smoke->findClass("QGraphicsRectItem");
    case 4:
        instance->classId = smoke->findClass("QGraphicsEllipseItem");
        break;
    case 5:
        instance->classId = smoke->findClass("QGraphicsPolygonItem");
        break;
    case 6:
        instance->classId = smoke->findClass("QGraphicsLineItem");
        break;
    case 7:
        instance->classId = smoke->findClass("QGraphicsItem");
        break;
    case 8:
        instance->classId = smoke->findClass("QGraphicsTextItem");
        break;
    case 9:
        instance->classId = smoke->findClass("QGraphicsSimpleTextItem");
        break;
    case 10:
        instance->classId = smoke->findClass("QGraphicsItemGroup");
        break;
    }
    
    return;
}

namespace JSmoke {
extern Marshall::TypeHandler QtGuiHandlers[];
extern void registerQtGuiTypes(QScriptEngine * engine);  
}

void qtscript_initialize_jsmoke_qtgui_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_qtgui_Smoke();
        JSmoke::Module qtgui_module = { "qtgui", new JSmoke::Binding(qtgui_Smoke) };
        JSmoke::Global::modules[qtgui_Smoke] = qtgui_module;    
        JSmoke::Global::QGraphicsItemClassId = qtcore_Smoke->idClass("QGraphicsItem");
        JSmoke::Marshall::installHandlers(JSmoke::QtGuiHandlers);
        JSmoke::Global::registerTypeResolver(    JSmoke::Global::QGraphicsItemClassId, 
                                                        qgraphicsitemTypeResolver );
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    JSmoke::Global::initializeClasses(engine, qtgui_Smoke);
    JSmoke::registerQtGuiTypes(engine);
    
    return;
}
