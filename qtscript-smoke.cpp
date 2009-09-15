#include "qtscript-smoke.h"

#include "StaticClass.h"
#include "QtScriptSmokeBinding.h"

#include "smoke/qt_smoke.h"

#include <QFile>
#include <QScriptEngine>
#include <QStringList>
#include <QtDebug>
#include <QTimer>

QtScriptSmoke::QtScriptSmoke()
{
    init_qt_Smoke();
    QTimer::singleShot( 0, this, SLOT( output() ) );
}

QtScriptSmoke::~QtScriptSmoke()
{}

void QtScriptSmoke::output()
{
    QScriptEngine* engine = new QScriptEngine( this );
    QScriptClass* qwidgetClass = new StaticClass( engine );
    QScriptValue qwidgetClassValue = engine->newObject( qwidgetClass );
    engine->globalObject().setProperty( "QWidget", qwidgetClassValue );
    
    qDebug() << "opening ../test.js";
    QFile testFile("../test.js");
    testFile.open( QFile::ReadOnly );
    QByteArray code = testFile.readAll();
    
    engine->evaluate( code, "test.js" );
    qDebug() << engine->isEvaluating();
    qDebug() << engine->hasUncaughtException();
    qDebug() << engine->uncaughtExceptionBacktrace();

}

#include "qtscript-smoke.moc"
