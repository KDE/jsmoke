/*
 * Copyright 2009 Ian Monroe <imonroe@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qtscript-smoke.h"

#include "marshall.h"
#include "object.h"
#include "metaobject.h"
#include "QtScriptSmokeBinding.h"
#include "SmokeQtScriptUtils.h"
#include "global.h"

#include "smoke/qtcore_smoke.h"
#include "smoke/qtnetwork_smoke.h"
#include "smoke/qtgui_smoke.h"

#include <QtGui/QApplication>

#include <QtCore/QFileInfo>
#include <QFile>
#include <QScriptEngine>
#include <QStringList>
#include <QtDebug>
#include <QTimer>

RunQtScriptSmoke::RunQtScriptSmoke(const QByteArray& script) : m_script(script)
{
    if (qtcore_Smoke == 0) {
        init_qtcore_Smoke();
    }

    QtScriptSmoke::Module qtcore_module = { "qtcore", new QtScriptSmoke::Binding(qtcore_Smoke) };
    QtScriptSmoke::Global::modules[qtcore_Smoke] = qtcore_module;
    
    if (qtgui_Smoke == 0) {
        init_qtgui_Smoke();
    }

    QtScriptSmoke::Module qtgui_module = { "qtgui", new QtScriptSmoke::Binding(qtgui_Smoke) };
    QtScriptSmoke::Global::modules[qtgui_Smoke] = qtgui_module;

    if (qtnetwork_Smoke == 0) {
        init_qtnetwork_Smoke();
    }

    QtScriptSmoke::Module qtnetwork_module = { "qtnetwork", new QtScriptSmoke::Binding(qtgui_Smoke) };
    QtScriptSmoke::Global::modules[qtnetwork_Smoke] = qtnetwork_module;

    QtScriptSmoke::installHandlers(QtScriptSmoke::Handlers);

    QtScriptSmoke::Global::QObjectClassId = qtcore_Smoke->idClass("QObject");
    QtScriptSmoke::Global::QDateClassId = qtcore_Smoke->idClass("QDate");
    QtScriptSmoke::Global::QDateTimeClassId = qtcore_Smoke->idClass("QDateTime");
    QtScriptSmoke::Global::QTimeClassId = qtcore_Smoke->idClass("QTime");
    
    QTimer::singleShot( 0, this, SLOT( output() ) );
}

RunQtScriptSmoke::~RunQtScriptSmoke()
{}

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

void
initializeClasses(QScriptEngine * engine, Smoke * smoke)
{
    for (int i = 1; i <= smoke->numTypes; i++) {
        printf("%s\n", smoke->types[i].name == 0 ? "void" : smoke->types[i].name);
    }
    
    for (int i = 1; i <= smoke->numClasses; i++) {
        // printf("className: %s\n", qt_Smoke->classes[i].className);
        QByteArray className(smoke->classes[i].className);        
        QScriptClass * klass;
        
        if (smoke->isDerivedFrom(   smoke, 
                                    i,
                                    QtScriptSmoke::Global::QObjectClassId.smoke,
                                    QtScriptSmoke::Global::QObjectClassId.index ) )
        {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    className, 
                                                    QtScriptSmoke::Global::SmokeQObject );
        } else {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    className, 
                                                    QtScriptSmoke::Global::Object );
        }
        
        if (className.contains("::")) {
            QStringList components = QString(className).split("::");
            QScriptValue outerClass = engine->globalObject().property(components[0]);
            
            for (int component = 1; component < components.length() - 1; ++component) {
                outerClass = outerClass.property(components[component]);
            }
            
            outerClass.setProperty(components.last(), engine->newObject(klass));
        } else {
            engine->globalObject().setProperty(QString(className), engine->newObject(klass));
        }
    }
}

void
RunQtScriptSmoke::output()
{
    QScriptEngine* engine = new QScriptEngine( this );
    QtScriptSmoke::registerTypes(engine);
    QtScriptSmoke::Global::Object = new QtScriptSmoke::Object(engine);
    QtScriptSmoke::Global::SmokeQObject = new QtScriptSmoke::SmokeQObject(engine);
    
    // QtScriptSmoke::Debug::DoDebug = QtScriptSmoke::Debug::Properties;
    initializeClasses(engine, qtcore_Smoke);
    initializeClasses(engine, qtgui_Smoke);
    initializeClasses(engine, qtnetwork_Smoke);
    
    QScriptValue QtClass = engine->globalObject().property(QString("Qt"));
    QtScriptSmoke::Global::QtEnum = engine->newFunction(QtEnum_ctor);
    QtClass.setProperty("Enum", QtScriptSmoke::Global::QtEnum);            
    QtClass.setProperty("Debug", engine->newFunction(Debug_ctor).call());

    QScriptValue app = QtScriptSmoke::Global::wrapInstance(engine, qtcore_Smoke->findClass("QApplication"), qApp);
    engine->globalObject().setProperty("qApp", app);
            
    qDebug() << "opening" << m_script;
    QFile testFile(m_script);
    QFileInfo fileInfo(testFile);
    testFile.open( QFile::ReadOnly );
    QByteArray code = testFile.readAll();
    
    QScriptValue result = engine->evaluate( code, fileInfo.fileName() );

    if (engine->hasUncaughtException()) {
        int line = engine->uncaughtExceptionLineNumber();
        qDebug() << "Uncaught exception at line" << line << ":" << engine->uncaughtException().toString();
        foreach (QString string, engine->uncaughtExceptionBacktrace()) {
            qDebug() << "backtrace>" << string;
        }
    }

}

#include "qtscript-smoke.moc"
