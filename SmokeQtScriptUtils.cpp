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

#include "SmokeQtScriptUtils.h"

#include "StaticClass.h"

#include <QByteArray>
#include <QScriptContext>
#include <QScriptValue>
#include <QVariant>
#include <QtDebug>

/* http://lists.kde.org/?l=kde-bindings&m=105167029023219&w=2
* The handler will first determine the Qt class hierarchy of the object (using 
* Smoke's idClass() and looking in the class hierarchy array) then build the 
* munged prototype of the requested method, following those rules:
* - take the requested method name
* - append $ for each simple native type argument (string, numeral, etc...) 
* - append  # for each Qt object passed as argument
* - append ? for things like an array, or a hash, or an undefined value     
*/
QVector<QByteArray> 
QtScriptSmoke::mungedMethods( const QByteArray& nameFn, QScriptContext* context )
{
        QVector<QByteArray> ret;
        ret.append( nameFn );
        
        for( int i = 0; i < context->argumentCount(); i++ )
        {
            QScriptValue val = context->argument( i );
            if( val.isNumber() || val.isBool() || val.isString() )
            {
                for (int i = 0; i < ret.count(); i++) {
                    ret[i] += '$';
                }
            }
            else if( val.isArray() )
            {
                for (int i = 0; i < ret.count(); i++) {
                    ret[i] += '?';
                }
            }
            else if( val.isNull() || val.isUndefined() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, ret) {
                    temp.append(mungedMethod + '$');
                    temp.append(mungedMethod + '?');
                    temp.append(mungedMethod + '#');
                }
                ret = temp;
            }
            else
            {
                for (int i = 0; i < ret.count(); i++) {
                    ret[i] += '#';
                }
            }
        }
        return ret;
}

// The code to match method arguments here, is based on callQtMethod() 
// in script/qscriptextqobject.cpp
//
QVector<QPair<Smoke::ModuleIndex, int> > 
QtScriptSmoke::resolveMethod(Smoke::ModuleIndex classId, const QByteArray& methodName, QScriptContext* context)
{
    Smoke::Class & klass = classId.smoke->classes[classId.index];
    QVector<Smoke::ModuleIndex> methodIds;
    QVector<QPair<Smoke::ModuleIndex, int> > matches;
    QVector<QByteArray> mungedMethods = QtScriptSmoke::mungedMethods(methodName, context);
        
    foreach (QByteArray mungedMethod, mungedMethods) {
        Smoke::ModuleIndex methodId = classId.smoke->findMethod(klass.className, mungedMethod);
        if (methodId.index == 0) {
            // We actually need to look in the QGlobalSpace of each open smoke module,
            // not just the one the current class is in. But we don't keep a list of
            // open smoke modules yet.
            Smoke * smoke = classId.smoke;
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
    
    if (methodIds.count() == 0) {
        return matches;
    }
    
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
    
    foreach (Smoke::ModuleIndex method, candidates) {
        Smoke::Method & methodRef = method.smoke->methods[method.index];
        
        if ((methodRef.flags & Smoke::mf_internal) == 0) {
            Smoke::Index *args = method.smoke->argumentList + methodRef.args;            
            const char *tname = method.smoke->types[methodRef.ret].name;            
            int matchDistance = 0;
            
            for (int i = 0; i < methodRef.numArgs; i++) {
                QScriptValue actual = context->argument(i);
                ushort argFlags = method.smoke->types[method.smoke->argumentList[methodRef.args+i]].flags;
                QByteArray argType(method.smoke->types[method.smoke->argumentList[methodRef.args+i]].name);
                argType.replace("const ", "").replace("&", "").replace("*", "");
                
                if (actual.isNumber()) {
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
                } else {
                    matchDistance += 10;
                }
            }
            
            if (matches.count() > 0 && matchDistance <= matches[0].second) {
                matches.prepend(QPair<Smoke::ModuleIndex, int>(method, matchDistance));
            } else {
                matches.append(QPair<Smoke::ModuleIndex, int>(method, matchDistance));
            }
        }
    }

    for (int i = 0; i < matches.count(); i++) {
        printf("resolveMethodSignature index: %d matchDistance: %d\n", matches[i].first.index, matches[i].second);
    }
    
    return matches;    
}
