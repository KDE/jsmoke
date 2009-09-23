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

#include "SmokeQtScriptUtils.h"
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
    qDebug() << "[ImplementationClass] queryProperty" << name << flags << id;
    if( name.toString() == "toString" )
        return 0;
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
    Smoke * smoke = attrObj->classId.smoke;
    Smoke::ModuleIndex classId = attrObj->classId;
    Smoke::Class & klass = classId.smoke->classes[classId.index];

    qDebug() << "callSmokeMethod " << klass.className << "::" << nameFn << " instance: " << attrObj->value;

    qDebug() << "[ImplementationClass] property called with argumentCount of:" << context->argumentCount();
    QVector<Smoke::ModuleIndex> methodIds;
    QVector<QByteArray> mungedMethods = SmokeQtScript::mungedMethods( nameFn.toLatin1(), context );
        
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
        qDebug() << "found a matching method meth: " << meth.method;
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
    SmokeQtScript::scriptArgumentsToSmoke( context, args );
    Smoke::ClassFn fn = smoke->classes[meth.classId].classFn;
    (*fn)(meth.method, attrObj->value, args);
    //TODO: return value
   return QScriptValue();
}

QScriptValue
ImplementationClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    QString nameStr = name;
    QScriptValue fn = engine()->newFunction( callSmokeMethod );
    fn.setData( QScriptValue( name ) );
    return fn;
}

QString
ImplementationClass::name() const
{
    return "ImplementationClass";
}

