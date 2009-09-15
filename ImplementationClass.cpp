/*
    QtScript-Smoke: Plays around with the idea of using smoke for qtscript
    Copyright (C) 2009 Ian Monroe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ImplementationClass.h"

#include "StaticClass.h"

#include <smoke/qt_smoke.h>

#include <QtDebug>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptValue>
#include <QStringList>

ImplementationClass::ImplementationClass( QScriptEngine* engine )
    : QScriptClass( engine )
{   }


ImplementationClass::~ImplementationClass()
{ }

QScriptClass::QueryFlags
ImplementationClass::queryProperty(const QScriptValue& object, const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    qDebug() << "[SampleImpl] queryProperty" << name << flags << id;
    return QScriptClass::HandlesReadAccess | QScriptClass::HandlesWriteAccess;

}

QScriptValue runSmokeMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    AttributedObject* attrObj = context->thisObject().data().toVariant().value<AttributedObject*>();
    qDebug() << "runSmokeMethod" << nameFn << attrObj;
    Smoke::ModuleIndex classId = qt_Smoke->findClass( attrObj->className );
    Smoke::Class klass = classId.smoke->classes[classId.index];
    

    qDebug() << "[ImplementationClass] property called with argumentCount of:" << context->argumentCount();
    Smoke::ModuleIndex methId;
    {
       /* http://lists.kde.org/?l=kde-bindings&m=105167029023219&w=2
        * The handler will first determine the Qt class hierarchy of the object (using 
        * Smoke's idClass() and looking in the class hierarchy array) then build the 
        * munged prototype of the requested method, following those rules:
        * - take the requested method name
        * - append $ for each simple native type argument (string, numeral, etc...) 
        * - append  # for each Qt object passed as argument
        * - append ? for things like an array, or a hash, or an undefined value     
        */
        QByteArray mungedArgs;
        for( int i = 0; i < context->argumentCount(); i++ )
        {
            QScriptValue val = context->argument( i );
            if( val.isNumber() || val.isBool() || val.isString() )
            {
                mungedArgs += '$';
            }
            else if( val.isArray() || val.isUndefined() )
            {
                mungedArgs += '?';
            }
            else
                mungedArgs += '#';
        }
        
        methId = qt_Smoke->findMethod( attrObj->className, nameFn.toLatin1()  + mungedArgs );
    }
    Smoke::Method meth = methId.smoke->methods[methId.smoke->methodMaps[methId.index].method];
    
    Smoke::StackItem args[context->argumentCount() + 1];
    for( int i = 0; i < context->argumentCount(); i++ )
    {
        QScriptValue val = context->argument( i );
        int argsPos = i + 1; //args[0] has the return value
        if( val.isBool() )
        {
            args[argsPos].s_bool = val.toBool();
            qDebug() << "bool arg" << args[i].s_bool;

        }
        else if( val.isNumber() )
        { //FIXME how do I know what kind of number? might have to look up function def
            args[argsPos].s_int = val.toInt32();
            qDebug() << "num arg" <<  args[i].s_int;

        }
        else if( val.isString() )
        {
            //FIXME memory management
            args[argsPos].s_voidp = new QString(val.toString());
            qDebug() << "string arg" << val.toString();
        }
        else 
        {
                qDebug() << "I don't know what this is." << val.toString();
        }
    }
    
    (*klass.classFn)(meth.method, attrObj->object, args);
    //TODO: return value
}

QScriptValue
ImplementationClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    QString nameStr = name;
    QScriptValue fn = engine()->newFunction( runSmokeMethod );
    fn.setData( QScriptValue( name ) );
    return fn;
    //decided to handle this in the function itself
    /*
    QByteArray className = object.data().toVariant().value<AttributedObject*>()->className;
    QScriptContext* context = engine()->currentContext();
    if( qt_Smoke->findMethod( (className + args).data(), nameStr.toLatin1() ).index != 0 )
    {
        qDebug() << "[ImplementationClass] calling method" << name;
        QScriptValue fn = engine()->newFunction( runSmokeMethod );
        fn.setData( QScriptValue( name ) );
        return fn;
    }
    else
    {
        qDebug() << "[ImplementationClass] could not find method" << name << args;
        return engine()->newObject();
    }
    */
}

QString
ImplementationClass::name() const
{
    return "ImplementationClass";
}

