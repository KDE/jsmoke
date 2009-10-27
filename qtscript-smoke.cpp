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

#include "smoke/qt_smoke.h"

#include <QtGui/QApplication>

#include <QtCore/QFileInfo>
#include <QFile>
#include <QScriptEngine>
#include <QStringList>
#include <QtDebug>
#include <QTimer>

RunQtScriptSmoke::RunQtScriptSmoke(const QByteArray& script) : m_script(script)
{
    init_qt_Smoke();
    QtScriptSmoke::Global::binding = QtScriptSmoke::Binding(qt_Smoke);    
    QtScriptSmoke::installHandlers(QtScriptSmoke::Handlers);

    QtScriptSmoke::Global::QObjectClassId = qt_Smoke->idClass("QObject");
    QtScriptSmoke::Global::QDateClassId = qt_Smoke->idClass("QDate");
    QtScriptSmoke::Global::QDateTimeClassId = qt_Smoke->idClass("QDateTime");
    QtScriptSmoke::Global::QTimeClassId = qt_Smoke->idClass("QTime");
    
    QTimer::singleShot( 0, this, SLOT( output() ) );
}

RunQtScriptSmoke::~RunQtScriptSmoke()
{}

QScriptValue 
RunQtScriptSmoke::includeQtClass(QScriptContext *context, QScriptEngine* engine) //STATIC
{
    if( context->argumentCount() == 1 && context->argument(0).isString() )
    {
        QByteArray className( context->argument(0).toString().toLatin1() );
        if( qt_Smoke->findClass(className).index != 0 )
        {
            QScriptClass* sclass = new QtScriptSmoke::MetaObject( engine, className, QtScriptSmoke::Global::Object );
            QScriptValue classValue = engine->newObject( sclass );
            engine->globalObject().setProperty(context->argument(0).toString(), classValue );
        }
        else
            context->throwError(QScriptContext::RangeError, className + " is not a supported class." );
    }
    else
       context->throwError(QScriptContext::TypeError, "Only one string argument for include.");

    return QScriptValue();
}

static QScriptValue QtEnum_valueOf(QScriptContext* context, QScriptEngine* engine)
{
    return context->thisObject().property("value");
}

/*
    The Qt.Enum class is used for marshalling enum values. It has a 'type' property
    with the type name of the enum, and a 'value' property. The 'type' name is used
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
     object.setProperty("type", context->argument(1));
     object.setProperty("valueOf", engine->newFunction(QtEnum_valueOf));
     return object;
}
 
void
RunQtScriptSmoke::output()
{
    QScriptEngine* engine = new QScriptEngine( this );
    QtScriptSmoke::registerTypes(engine);
    QtScriptSmoke::Global::Object = new QtScriptSmoke::Object(engine);
    QtScriptSmoke::Global::SmokeQObject = new QtScriptSmoke::SmokeQObject(engine);
    QScriptValue includeFn = engine->newFunction( RunQtScriptSmoke::includeQtClass, 1 );
    engine->globalObject().setProperty( "include", includeFn );
    
    // QtScriptSmoke::Debug::DoDebug |= QtScriptSmoke::Debug::Ambiguous;
    // QtScriptSmoke::Debug::DoDebug |= QtScriptSmoke::Debug::Properties;
    // QtScriptSmoke::Debug::DoDebug |= QtScriptSmoke::Debug::Calls;
    // QtScriptSmoke::Debug::DoDebug |= QtScriptSmoke::Debug::GC;
    // QtScriptSmoke::Debug::DoDebug |= QtScriptSmoke::Debug::Virtual;
    
    
    /*
     Try timing this code with 'time ./qtscript-smoke'. On my slow netbook I got 
     this time with the loop:
     
        real    0m3.916s
        user    0m0.340s
        sys     0m0.084s

     Without the loop I got:
     
        real    0m3.295s
        user    0m0.332s
        sys     0m0.072s

     So as far a I can see, creating the classes on startup is pretty cheap.
     
     -- Richard
    */
    for (int i = 1; i <= qt_Smoke->numClasses; i++) {
        // printf("className: %s\n", qt_Smoke->classes[i].className);
        
        QScriptClass * klass;
        if (qt_Smoke->isDerivedFrom(    qt_Smoke, 
                                        i,
                                        QtScriptSmoke::Global::QObjectClassId.smoke,
                                        QtScriptSmoke::Global::QObjectClassId.index ) )
        {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    qt_Smoke->classes[i].className, 
                                                    QtScriptSmoke::Global::SmokeQObject );
        } else {
            klass = new QtScriptSmoke::MetaObject(  engine, 
                                                    qt_Smoke->classes[i].className, 
                                                    QtScriptSmoke::Global::Object );
        }
        
        QScriptValue classValue = engine->newObject(klass);
        engine->globalObject().setProperty(QString(qt_Smoke->classes[i].className), classValue);
  
        if (qstrcmp(qt_Smoke->classes[i].className, "Qt") == 0) {
            QtScriptSmoke::Global::QtEnum = engine->newFunction(QtEnum_ctor);
            classValue.setProperty("Enum", QtScriptSmoke::Global::QtEnum);
        }
    }
  
    QScriptValue app = QtScriptSmoke::Global::wrapInstance(engine, qt_Smoke->findClass("QApplication"), qApp);
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
