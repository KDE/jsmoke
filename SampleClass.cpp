#include "SampleClass.h"
#include "SampleImpl.h"

#include "smoke/qt_smoke.h"

#include <QtDebug>
#include <QScriptContext>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptString>
#include <QVariant>

SampleClass::SampleClass( QScriptEngine* engine )
    : QScriptClass( engine )
    , m_protoClass( new SampleImpl( engine ) )
{ }

SampleClass::~SampleClass()
{  }

QScriptValue
SampleClass::prototype() const
{
    qDebug("this function shouldn't be called");
    return QScriptValue();
}

QScriptClass::QueryFlags
SampleClass::queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id )
{
    //qDebug() << object.toVariant();
    qDebug() << "queryProperty" << name << flags << id;
    if( engine()->toStringHandle("prototype") == name )
    {
      //  return QScriptClass::HandlesReadAccess;
        return 0;
    }
    else
        return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;
}

QScriptValue stuff(QScriptContext *context, QScriptEngine* engine)
{
    QScriptValue ret = engine->newObject();
    qDebug() << "calling a C++ function!!";
    return ret;
}

QScriptValue
SampleClass::property ( const QScriptValue & object, const QScriptString & name, uint id )
{
    qDebug() << "property" << name << id;
    if( name == engine()->toStringHandle("stuff") )
        return engine()->newFunction( stuff );
    else if( name == engine()->toStringHandle("prototype") )
    {
        qDebug() << "its asking for the prototype";
        //return m_proto;
        return engine()->newObject();
    }
    return engine()->newObject();
}

QVariant
SampleClass::extension( QScriptClass::Extension extension, const QVariant& argument )
{
    if( extension == Callable )
    {
        //QScriptContext* context = qvariant_cast<QScriptContext*>( argument );
        qDebug() << "now we run the QWidget constructor";
        QScriptContext* context = argument.value<QScriptContext*>();
        qDebug() << "constructor?" << context->isCalledAsConstructor();
        Smoke::ModuleIndex classId = qt_Smoke->findClass("QWidget");
        Smoke::Class klass = classId.smoke->classes[ classId.index ];
        
        Smoke::ModuleIndex methId = qt_Smoke->findMethod("QWidget", "QWidget");
        Smoke::Method meth = methId.smoke->methods[methId.smoke->methodMaps[methId.index].method];
        
        Smoke::StackItem stack[1];
        (*klass.classFn)(meth.method, 0, stack);
        void *widget = stack[0].s_voidp;
        QScriptValue proto = engine()->newObject( m_protoClass );
        
        AttributedObject* attrObj = new AttributedObject();
        attrObj->className = "QWidget";
        attrObj->object = widget;
        proto.setData( engine()->newVariant( QVariant::fromValue<AttributedObject*>( attrObj ) ) );
        
        context->setThisObject(proto);
        return 15;
    }
}

bool
SampleClass::supportsExtension( QScriptClass::Extension extension ) const
{
        if( extension == Callable )
            return true;
}


