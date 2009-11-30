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

#include <QtCore/QByteArray>                                                                                                              
#include <QtCore/QVariant>

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

#include <global.h>
#include <marshall.h>
#include <marshallmacros.h>
#include <utils.h>

#include <smoke/qtcore_smoke.h>

static QScriptValue QtEnum_toString(QScriptContext* context, QScriptEngine* engine)
{
    return context->thisObject().property("value").toString();
}

static QScriptValue QtEnum_valueOf(QScriptContext* context, QScriptEngine* engine)
{
    return context->thisObject().property("value");
}

/*
    The Qt.Enum class is used for marshalling enum values. It has a 'typeName' property
    with the type name of the enum, and a 'value' property. The 'typeName' is used
    when overloaded methods need to be resolved on enum types of the arguments.
 */
static QScriptValue QtEnum_ctor(QScriptContext* context, QScriptEngine* engine)
{
     QScriptValue object;
     if (context->isCalledAsConstructor()) {
         object = context->thisObject();
     } else {
         object = engine->newObject();
         object.setPrototype(context->callee().property("prototype"));
     }
     
     object.setProperty("value", context->argument(0));
     object.setProperty("typeName", context->argument(1));
     object.setProperty("valueOf", engine->newFunction(QtEnum_valueOf));
     object.setProperty("toString", engine->newFunction(QtEnum_toString));
     return object;
}

static QScriptValue Debug_trace(QScriptContext* context, QScriptEngine* engine)
{
    QtScriptSmoke::Debug::DoDebug = context->argument(0).toUInt32();
    return engine->undefinedValue();
}

static QScriptValue Debug_ctor(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue object = engine->newObject();
    object.setPrototype(context->callee().property("prototype"));
     
    object.setProperty("None", 0);
    object.setProperty("MethodMatches", 1);
    object.setProperty("Properties", 2);
    object.setProperty("Calls", 4);
    object.setProperty("GC", 8);
    object.setProperty("Virtual", 16);
    object.setProperty("Verbose", 32);
    
    object.setProperty("trace", engine->newFunction(Debug_trace), QScriptValue::PropertySetter);
    return object;
}

static QScriptValue 
QVariant_valueOf(QScriptContext* context, QScriptEngine* engine)
{
    QtScriptSmoke::Object::Instance * instance = QtScriptSmoke::Object::Instance::get(context->thisObject());
    QVariant * variant = static_cast<QVariant*>(instance->value);
    // printf("QVariant_value() instance->value: %p typeName: %s userType: %d className: %s\n", 
    //       instance->value, variant->typeName(), variant->userType(), 
    //       instance->classId.smoke->classes[instance->classId.index].className);
    if (QByteArray(instance->classId.smoke->classes[instance->classId.index].className) != "QVariant"
        || variant->typeName() == 0 ) 
    {
        return context->thisObject();
    }
    
    return QtScriptSmoke::valueFromVariant(engine, *variant);
}

static QScriptValue 
QVariant_fromValue(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue value = context->argument(0);
    QVariant variant = QtScriptSmoke::valueToVariant(value);
    QVariant * copy = new QVariant(variant);
    QScriptValue result = QtScriptSmoke::Global::wrapInstance(engine, qtcore_Smoke->findClass("QVariant"), copy);
    return result;
}

namespace QtScriptSmoke {
extern Marshall::TypeHandler QtCoreHandlers[];
extern void registerQtCoreTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_qt_core_bindings(QScriptValue& extensionObject)
{
    QScriptEngine* engine = extensionObject.engine();
    init_qtcore_Smoke();
    
    QtScriptSmoke::Global::Object = new QtScriptSmoke::Object(engine);
    QtScriptSmoke::Global::SmokeQObject = new QtScriptSmoke::SmokeQObject(engine);
    QtScriptSmoke::Global::initializeClasses(engine, qtcore_Smoke);
    
    QtScriptSmoke::Module qtcore_module = { "qtcore", new QtScriptSmoke::Binding(qtcore_Smoke) };
    QtScriptSmoke::Global::modules[qtcore_Smoke] = qtcore_module;    
    QtScriptSmoke::Marshall::installHandlers(QtScriptSmoke::QtCoreHandlers);
    
    QtScriptSmoke::Global::QObjectClassId = qtcore_Smoke->idClass("QObject");
    QtScriptSmoke::Global::QDateClassId = qtcore_Smoke->idClass("QDate");
    QtScriptSmoke::Global::QDateTimeClassId = qtcore_Smoke->idClass("QDateTime");
    QtScriptSmoke::Global::QTimeClassId = qtcore_Smoke->idClass("QTime");
        
    QScriptValue QtClass = engine->globalObject().property(QString("Qt"));
    QtScriptSmoke::Global::QtEnum = engine->newFunction(QtEnum_ctor);
    QtClass.setProperty("Enum", QtScriptSmoke::Global::QtEnum);            
    QtClass.setProperty("Debug", engine->newFunction(Debug_ctor).call());

    QScriptValue QVariantClass = engine->globalObject().property(QString("QVariant"));
    QVariantClass.setProperty("fromValue", engine->newFunction(QVariant_fromValue));
    QVariantClass.property("prototype").setProperty("valueOf", engine->newFunction(QVariant_valueOf));
    QtScriptSmoke::registerQtCoreTypes(engine);
    
    return;
}
