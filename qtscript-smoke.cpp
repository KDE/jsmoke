#include "qtscript-smoke.h"

#include "SampleClass.h"
#include "QtScriptSmokeBinding.h"

#include "smoke/qt_smoke.h"

#include <QScriptEngine>
#include <QStringList>
#include <QtDebug>
#include <QTimer>

QtScriptSmoke::QtScriptSmoke()
{
    init_qt_Smoke();
    new QtScriptSmokeBinding( qt_Smoke );
    QTimer::singleShot( 0, this, SLOT( output() ) );
}

QtScriptSmoke::~QtScriptSmoke()
{}

/*QScriptValue attributedObjectToScriptValue( QScriptEngine *engine, const AttributedObject& obj)
{
    QScriptValue ret = engine->newObject();
    ret.setData( engine->toScriptValue<AttributedObject*>( &obj ) );
    return ret;
}

void scriptValueToAttributedObject( const QScriptValue &val, AttributedObject& obj)
{
    obj = qscriptvalue_cast<AttributedObject*>( &val.data() );
}*/

void QtScriptSmoke::output()
{
    QScriptEngine* engine = new QScriptEngine( this );
  //  qScriptRegisterMetaType( engine, attributedObjectToScriptValue, scriptValueToAttributedObject );
    QScriptClass* sample = new SampleClass( engine );
    QScriptValue scriptSample = engine->newObject( sample );
    engine->globalObject().setProperty( "SampleClass", scriptSample );
    QString code;
    //code += "try {";
    //code += "SampleClass.stuff();";
    //code += "SampleClass.apple = \"delicious\";";
    //code += "SampleClass();";
    code += "hello = new SampleClass();";
    code += "hello.show();";

    //code += "} catch(error) { print error; }";
    engine->evaluate( code, "samplecode.js" );
    qDebug() << engine->isEvaluating();
    qDebug() << engine->isEvaluating();
    qDebug() << engine->hasUncaughtException();
    qDebug() << engine->uncaughtExceptionBacktrace();

}

#include "qtscript-smoke.moc"
