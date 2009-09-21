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

// The code to match method arguments here, is based on callQtMethod() 
// in script/qscriptextqobject.cpp
//
QVector<Smoke::ModuleIndex> resolve(QScriptContext* context, const QVector<Smoke::ModuleIndex>& candidates)
{
    foreach (Smoke::ModuleIndex method, candidates) {
        Smoke::Method & methodRef = method.smoke->methods[method.index];
        
        if ((methodRef.flags & Smoke::mf_internal) == 0) {
            Smoke::Index *args = method.smoke->argumentList + methodRef.args;            
            const char *tname = method.smoke->types[methodRef.ret].name;
            printf("return type: %s\n", (tname ? tname : "void"));
            printf("method name: %s::%s\n", method.smoke->classes[methodRef.classId].className, method.smoke->methodNames[methodRef.name]);
            
            bool converted = true;
            int matchDistance = 0;
            for (int i = 0; i < methodRef.numArgs; i++) {
                QScriptValue actual = context->argument(i);
                ushort argFlags = method.smoke->types[method.smoke->argumentList[methodRef.args+i]].flags;
                QByteArray argType(method.smoke->types[method.smoke->argumentList[methodRef.args+i]].name);
                argType.replace("const ", "").replace("&", "").replace("*", "");
                
                if(actual.isNumber()) {
                    switch (argFlags & Smoke::tf_elem) {
                    case Smoke::t_double:
                        // perfect
                        break;
                    case Smoke::t_float:
                        matchDistance += 1;
                        break;
                    case Smoke::t_long:
                    case Smoke::t_ulong:
                        matchDistance += 3;
                        break;
                    case Smoke::t_int:
                    case Smoke::t_uint:
                        matchDistance += 4;
                        break;
                    case Smoke::t_short:
                    case Smoke::t_ushort:
                        matchDistance += 5;
                        break;
                    case Smoke::t_char:
                    case Smoke::t_uchar:
                        matchDistance += 6;
                        break;
                    default:
                        matchDistance += 10;
                        break;
                    }
                } else if (actual.isString()) {
                    if (argType == "QString") {
                    } else {
                        matchDistance += 10;
                    }
                } else if (actual.isBool()) {
                    if ((argFlags & Smoke::tf_elem) == Smoke::t_bool) {
                    } else {
                        matchDistance += 10;
                    }
                } else if (actual.isDate()) {
                    if (argType == "QDataTime") {
                    } else if (argType == "QDate") {
                        matchDistance += 1;
                    } else if (argType == "QTime") {
                        matchDistance += 2;
                    } else {
                        matchDistance += 10;
                    }
                } else if (actual.isRegExp()) {
                    if (argType == "QRegExp") {
                    } else {
                        matchDistance += 10;
                    }
                } else if (actual.isVariant()) {
                } else if (actual.isArray()) {
                } else if (actual.isQObject()) {
                } else if (actual.isNull()) {
                } else if (actual.isObject()) {
                }
            }
        }
    }
}

QScriptValue callSmokeMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    QtScript::SmokeInstance * attrObj = QtScript::SmokeInstance::get(context->thisObject());
    qDebug() << "callSmokeMethod" << nameFn << attrObj;
    Smoke * smoke = attrObj->classId.smoke;
    Smoke::ModuleIndex classId = attrObj->classId;
    Smoke::Class & klass = classId.smoke->classes[classId.index];
    

    qDebug() << "[ImplementationClass] property called with argumentCount of:" << context->argumentCount();
    QVector<Smoke::ModuleIndex> methodIds;
    {
       /* http://lists.kde.org/?l=kde-bindings&m=105167029023219&w=2
        * The handler will first determine the Qt class hierarchy of the object (using 
        * Smoke's idClass() and looking in the class hierarchy array) then build the 
        * munged prototype of the requested method, following those rules:
        * - take the requested method name
        * - append $ for each simple native type argument (string, numeral, etc...) 
        * - append  # for each Qt object passed as argument
        * - append ? for things like an array, or a hash, or an undefined value  
        *
        * Note that there may be more than one munged method name if there are any
        * null arguments, as null could be either a '$', '?' or '#' munged arg type
        */
        QVector<QByteArray> mungedMethods;
        mungedMethods.append(nameFn.toLatin1());
        
        for( int i = 0; i < context->argumentCount(); i++ )
        {
            QScriptValue val = context->argument( i );
            if( val.isNumber() || val.isBool() || val.isString() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, mungedMethods) {
                    temp.append(mungedMethod + '$');
                }
                mungedMethods = temp;
            }
            else if( val.isArray() || val.isUndefined() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, mungedMethods) {
                    temp.append(mungedMethod + '?');
                }
                mungedMethods = temp;
            }
            else if( val.isNull() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, mungedMethods) {
                    temp.append(mungedMethod + '$');
                    temp.append(mungedMethod + '?');
                    temp.append(mungedMethod + '#');
                }
                mungedMethods = temp;
            }
            else
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, mungedMethods) {
                    temp.append(mungedMethod + '#');
                }
                mungedMethods = temp;
            }
        }
        
        foreach (QByteArray mungedMethod, mungedMethods) {
            Smoke::ModuleIndex methodId = smoke->findMethod(klass.className, mungedMethod);
            if (methodId.index == 0) {
                // We actually need to look in the QGlobalSpace of each open smoke module,
                // not just the one the current class is in. But we don't keep a list of
                // open smoke modules yet.
                Smoke::ModuleIndex globalClassId = smoke->idClass("QGlobalSpace");
                Smoke::ModuleIndex globalMethodId = smoke->idMethodName(mungedMethod);
                if (globalClassId.index != 0 && globalMethodId.index != 0) {
                    methodId = smoke->idMethod(globalClassId.index, globalMethodId.index);
                    methodIds.append(methodId);
                }
            } else {
                methodIds.append(methodId);
            }
        }
    }
    
    Smoke::Method meth;
    QVector<Smoke::ModuleIndex> candidates;
    
    foreach (Smoke::ModuleIndex methodId, methodIds) {
        Smoke::Index ix = methodId.smoke->methodMaps[methodId.index].method;
        Smoke::ModuleIndex mi;
        if (ix == 0) {
        } else if (ix > 0) {
            mi.index = ix;
            mi.smoke = methodId.smoke;
            candidates.append(mi);
        } else if (ix < 0) {
            ix = -ix;
            while (methodId.smoke->ambiguousMethodList[ix] != 0) {
                mi.index = methodId.smoke->ambiguousMethodList[ix];
                mi.smoke = methodId.smoke;
                candidates.append(mi);
                ix++;
            }
        }        
    }

    // Really every call should go through resulve() even if there is only one
    // candidate, to check that the arguments match. However, special case the single
    // candidate case while resolve() is being finished, so that the test example 
    // carries on working
    if (candidates.count() == 1) {
        meth = candidates[0].smoke->methods[candidates[0].index];
    } else {
        QVector<Smoke::ModuleIndex> matches = resolve(context, candidates);
        if (matches.count() == 0) {
            // Error
        } else if (matches.count() > 1) {
            // Error
        } else {
            meth = matches[0].smoke->methods[matches[0].index];
        }
    }
    
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
    
    (*klass.classFn)(meth.method, attrObj->value, args);
    //TODO: return value
}

QScriptValue
ImplementationClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    QString nameStr = name;
    QScriptValue fn = engine()->newFunction( callSmokeMethod );
    fn.setData( QScriptValue( name ) );
    return fn;
    //decided to handle this in the function itself
    /*
    QByteArray className = object.data().toVariant().value<AttributedObject*>()->className;
    QScriptContext* context = engine()->currentContext();
    if( qt_Smoke->findMethod( (className + args).data(), nameStr.toLatin1() ).index != 0 )
    {
        qDebug() << "[ImplementationClass] calling method" << name;
        QScriptValue fn = engine()->newFunction( callSmokeMethod );
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

