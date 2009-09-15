#include "StaticClass.h"

#include "ImplementationClass.h"
#include "QtScriptSmokeBinding.h"

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptString>
#include <QVariant>

StaticClass::StaticClass( QScriptEngine* engine )
    : QScriptClass( engine )
    , m_protoClass( new ImplementationClass( engine ) )
{ }

StaticClass::~StaticClass()
{  }

QScriptValue
StaticClass::prototype() const
{
    //this fn is called, but i'm pretty sure this function doesn't 
    //do anything, due to the Callable extension
    return QScriptValue();
}

QScriptClass::QueryFlags
StaticClass::queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id )
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
StaticClass::property ( const QScriptValue & object, const QScriptString & name, uint id )
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
StaticClass::extension( QScriptClass::Extension extension, const QVariant& argument )
{
    if( extension == Callable )
    {
        //QScriptContext* context = qvariant_cast<QScriptContext*>( argument );
        qDebug() << "now we run the QWidget constructor";
        QScriptContext* context = argument.value<QScriptContext*>();
        qDebug() << "constructor?" << context->isCalledAsConstructor() << context->backtrace();
        //TODO: obviously we'll want to make more objects then QWidget 
        //the current plan is to have a "include" global function that creates
        //a new global QScriptValue from a StaticClass for the class specified
        //some ninja way to allow all class QScriptValue's to use the same StaticClass 
        //would be nice, it only requires that this function know the name of the classes
        //its supposed to create
        
        //I tried the following ninjutsu, but the functionName is blank
        //QScriptContextInfo cInfo( context );
        //QByteArray classNameByteArray = cInfo.functionName().toLatin1();
        //const char* className = classNameByteArray.constData();
        //qDebug() << "we find the classname to be" << className
        
        const char* className = "QWidget";
        Smoke::ModuleIndex classId = qt_Smoke->findClass(className);
        Smoke::Class klass = classId.smoke->classes[ classId.index ];
        
        //TODO constructor arguments
        Smoke::ModuleIndex methId = qt_Smoke->findMethod(className, className);
        Smoke::Method meth = methId.smoke->methods[methId.smoke->methodMaps[methId.index].method];
        
        Smoke::StackItem stack[1];
        (*klass.classFn)(meth.method, 0, stack);
        void *widget = stack[0].s_voidp;
        
        Smoke::StackItem initializeInstanceStack[2];
        initializeInstanceStack[1].s_voidp = &g_binding;
        //0 is a special method to initaliaze an instance
        (*klass.classFn)(0, widget, initializeInstanceStack);
        
        
        QScriptValue proto = engine()->newObject( m_protoClass );
        
        AttributedObject* attrObj = new AttributedObject();
        attrObj->className = className;
        attrObj->object = widget;
        proto.setData( engine()->newVariant( QVariant::fromValue<AttributedObject*>( attrObj ) ) );
        
        context->setThisObject(proto);
        return 15;
    }
}

bool
StaticClass::supportsExtension( QScriptClass::Extension extension ) const
{
        if( extension == Callable )
            return true;
}


