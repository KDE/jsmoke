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

#include <QtCore/QByteArray>
#include <QtCore/QVariant>
#include <QtCore/QtDebug>
#include <QtCore/QDateTime>
#include <QtCore/QRegExp>

#include <QtScript/QScriptContext>
#include <QtScript/QScriptContextInfo>
#include <QtScript/QScriptValue>

#include "smoke/qtcore_smoke.h"

#include "utils.h"
#include "marshall.h"
#include "methodcall.h"
#include "global.h"

namespace QtScriptSmoke {

QString
methodToString(Smoke::ModuleIndex methodId)
{
    QString result;
    Smoke * smoke = methodId.smoke;
    Smoke::Method& methodRef = smoke->methods[methodId.index];
    const char * typeName = smoke->types[methodRef.ret].name;
    
    if ((methodRef.flags & Smoke::mf_enum) != 0) {
        result.append(QString("enum %1::%2")
                            .arg(smoke->classes[methodRef.classId].className)
                            .arg(smoke->methodNames[methodRef.name]) );
        return result;
    }
    
    if ((methodRef.flags & Smoke::mf_static) != 0) {
        result.append("static ");
    }
    
    result.append((typeName != 0 ? typeName : "void"));
    result.append(  QString(" %1::%2(")
                        .arg(smoke->classes[methodRef.classId].className)
                        .arg(smoke->methodNames[methodRef.name]) );
                        
    for (int i = 0; i < methodRef.numArgs; i++) {
        if (i > 0) {
            result.append(", ");
        }
        
        typeName = smoke->types[smoke->argumentList[methodRef.args+i]].name;
        result.append((typeName != 0 ? typeName : "void"));
    }
    
    result.append(")");
    if ((methodRef.flags & Smoke::mf_const) != 0) {
        result.append(" const");
    }
    
    return result;
}

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
mungedMethods( const QByteArray& nameFn, QScriptContext* context )
{
        QVector<QByteArray> ret;
        ret.append( nameFn );
        
        for( int i = 0; i < context->argumentCount(); i++ )
        {
            QScriptValue val = context->argument( i );
            if (    val.isNumber() 
                    || val.isBool() 
                    || val.isString() 
                    || val.instanceOf(QtScriptSmoke::Global::QtEnum) )
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
            } else if ( Object::Instance::isSmokeObject(val) 
                        || val.isDate()
                        || val.isRegExp() )
            {
                for (int i = 0; i < ret.count(); i++) {
                    ret[i] += '#';
                }
            } else {
                qDebug() << "Unknown value type:" << val.toString();
            }
        }
        return ret;
}

// The code to match method arguments here, is based on callQtMethod() 
// in script/qscriptextqobject.cpp
//
QVector<QPair<Smoke::ModuleIndex, int> > 
resolveMethod(Smoke::ModuleIndex classId, const QByteArray& methodName, QScriptContext* context)
{
    Smoke::Class & klass = classId.smoke->classes[classId.index];
    QVector<Smoke::ModuleIndex> methodIds;
    QVector<QPair<Smoke::ModuleIndex, int> > matches;
    QVector<QByteArray> mungedMethods = QtScriptSmoke::mungedMethods(methodName, context);
        
    foreach (QByteArray mungedMethod, mungedMethods) {
        Smoke::ModuleIndex methodId = classId.smoke->findMethod(klass.className, mungedMethod);

        if (methodId.index == 0) {
            // Look in all the open smoke modules
            QHashIterator<Smoke*, Module> it(Global::modules);
            while (it.hasNext()) {
                it.next();
                methodId = it.key()->findMethod("QGlobalSpace", mungedMethod);
                
                if (methodId.index != 0) {
                    methodIds.append(methodId);
                }
            }
        } else {
            methodIds.append(methodId);
        }
    }
    
    if (methodIds.count() == 0) {
        if ((Debug::DoDebug & Debug::MethodMatches) != 0) {
            QScriptContextInfo contextInfo(context->parentContext());
            qWarning(   "Method matches@%s:%d for %s.%s(): No matches", 
                        contextInfo.fileName().toLatin1().constData(),
                        contextInfo.lineNumber(),
                        klass.className,
                        methodName.constData());
        }
        
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
            
            // If a method is overloaded only on const-ness, prefer the
            // non-const version
            if ((methodRef.flags & Smoke::mf_const) != 0) {
                matchDistance += 1;
            }
            
            for (int i = 0; i < methodRef.numArgs; i++) {
                QScriptValue actual = context->argument(i);
                ushort argFlags = method.smoke->types[method.smoke->argumentList[methodRef.args+i]].flags;
                QByteArray fullArgType(method.smoke->types[method.smoke->argumentList[methodRef.args+i]].name);
                fullArgType.replace("const ", "");
                QByteArray argType(fullArgType);
                argType.replace("&", "").replace("*", "");
                
                if (actual.isNumber()) {
                    switch (argFlags & Smoke::tf_elem) {
                    case Smoke::t_enum:
                        matchDistance += 1;
                        break;
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
                } else if (actual.instanceOf(QtScriptSmoke::Global::QtEnum)) {
                    switch (argFlags & Smoke::tf_elem) {
                    case Smoke::t_enum:
                        if (actual.property("typeName").toString().toLatin1() != argType) {
                            matchDistance += 10;
                        }
                        break;
                    case Smoke::t_uint:
                        matchDistance += 1;
                        break;                    
                    case Smoke::t_int:
                        matchDistance += 2;
                        break;                    
                    case Smoke::t_long:
                    case Smoke::t_ulong:
                        matchDistance += 3;
                        break;
                    default:
                        matchDistance += 10;
                        break;
                    }
                } else if (actual.isString()) {
                    if (argType == "QString") {
                    } else if (fullArgType == "char*" || fullArgType == "unsigned char*") {
                        matchDistance += 1;
                    } else if (argType == "QChar") {
                        matchDistance += 2;
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
                } else if (Object::Instance::isSmokeObject(actual)) {
                    if ((argFlags & Smoke::t_class) != 0) {
                        Object::Instance * instance = Object::Instance::get(actual);
                        Smoke::ModuleIndex classId = qtcore_Smoke->findClass(argType);

                        if (instance->classId == classId) {
                        } else if ( qtcore_Smoke->isDerivedFrom(    instance->classId.smoke, 
                                                                    instance->classId.index,
                                                                    classId.smoke,
                                                                    classId.index ) )
                        {
                            matchDistance += 1;
                        } else {
                            matchDistance += 10;
                        }
                    } else {
                        matchDistance += 10;
                    }
                } else if (actual.isVariant()) {
                } else if (actual.isArray()) {
                    if (    argType.contains("QVector") 
                            || argType.contains("QList")
                            || argType.contains("QStringList")) {
                    } else {
                        matchDistance += 10;
                    }
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

    if ((Debug::DoDebug & Debug::MethodMatches) != 0) {
        QScriptContextInfo contextInfo(context->parentContext());
        qWarning(   "Method matches@%s:%d for %s.%s():", 
                    contextInfo.fileName().toLatin1().constData(),
                    contextInfo.lineNumber(),
                    klass.className, 
                    methodName.constData());
        for (int i = 0; i < matches.count(); i++) {
            qWarning("    %s module: %s index: %d matchDistance: %d", 
                methodToString(matches[i].first).toLatin1().constData(),
                matches[i].first.smoke->moduleName(), 
                matches[i].first.index, 
                matches[i].second);
        }
    }
    
    return matches;    
}

QScriptValue 
callSmokeStaticMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    QScriptClass * cls = context->thisObject().scriptClass();
    Smoke::ModuleIndex classId = static_cast<MetaObject*>(cls)->classId();
    QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(classId, nameFn.toLatin1(), context);
    
    if (matches.count() == 0) {
        QString message = QString("%1 is not defined").arg(nameFn);
        return context->throwError(QScriptContext::ReferenceError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        // Error if the first two have the same matchDistance
        QString message = QString("overloaded %1() call not resolved").arg(nameFn);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }
    
    QtScriptSmoke::MethodCall methodCall(matches[0].first.smoke, matches[0].first.index, context, engine);
    methodCall.next();
    return *(methodCall.var());
}

QScriptValue 
callSmokeMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    Object::Instance * instance = Object::Instance::get(context->thisObject());
    
    QVector<QPair<Smoke::ModuleIndex, int> > matches = QtScriptSmoke::resolveMethod(instance->classId, nameFn.toLatin1(), context);
    
    if (matches.count() == 0) {
        QString message = QString("%1 is not defined").arg(nameFn);
        return context->throwError(QScriptContext::ReferenceError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        // Error if the first two have the same matchDistance
        QString message = QString("overloaded %1() call not resolved").arg(nameFn);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }
    
    QtScriptSmoke::MethodCall methodCall(matches[0].first.smoke, matches[0].first.index, context, engine);
    methodCall.next();
    return *(methodCall.var());
}

QScriptValue 
instanceToString(QScriptContext* context, QScriptEngine* engine)
{
    Object::Instance * instance = Object::Instance::get(context->thisObject());
    Smoke::Class & klass = instance->classId.smoke->classes[instance->classId.index];
    QString str = QString("[object %1:0x%2]")
        .arg(QString(klass.className).replace("::", "."))
        .arg(reinterpret_cast<ulong>(instance->value), 8, 16, QLatin1Char('0'));
    return QScriptValue(engine, str);
}

void *
constructCopy(Object::Instance* instance)
{
    Smoke * smoke = instance->classId.smoke;
    const char *className = smoke->className(instance->classId.index);
    
    QByteArray ccSignature(className);
    int pos = ccSignature.lastIndexOf("::");    
    if (pos != -1) {
        ccSignature = ccSignature.mid(pos + strlen("::"));
    }    
    ccSignature.append("#");
    
    QByteArray ccArg("const ");
    ccArg.append(className).append("&");

    Smoke::ModuleIndex ccId = smoke->findMethodName(className, ccSignature);
    Smoke::ModuleIndex ccMethod = smoke->findMethod(instance->classId, ccId);

    if (ccMethod.index == 0) {
        qWarning("QtScriptSmoke::constructCopy() failed %s %p\n", className, instance->value);
        return 0;
    }
    
    Smoke::Index method = ccMethod.smoke->methodMaps[ccMethod.index].method;
    if (method > 0) {
        // Make sure it's a copy constructor
        if (ccArg != smoke->types[*(smoke->argumentList + smoke->methods[method].args)].name) {
            qWarning("QtScriptSmoke::constructCopy() failed %s %p\n", className, instance->value);
            return 0;
        }

        ccMethod.index = method;
    } else {
        // ambiguous method, pick the copy constructor
        Smoke::Index i = -method;
        while (ccMethod.smoke->ambiguousMethodList[i]) {
            if (ccArg == smoke->types[*(smoke->argumentList + smoke->methods[ccMethod.smoke->ambiguousMethodList[i]].args)].name) {
                break;
            }
            i++;
        }

        ccMethod.index = ccMethod.smoke->ambiguousMethodList[i];
        if (ccMethod.index == 0) {
            qWarning("QtScriptSmoke::constructCopy() failed %s %p\n", className, instance->value);
            return 0;
        }
    }

    // Okay, ccMethod is the copy constructor. Time to call it.
    Smoke::StackItem args[2];
    args[0].s_voidp = 0;
    args[1].s_voidp = instance->value;
    Smoke::ClassFn fn = smoke->classes[instance->classId.index].classFn;
    (*fn)(smoke->methods[ccMethod.index].method, 0, args);

    // Initialize the binding for the new instance
    Smoke::StackItem initializeInstanceStack[2];
    initializeInstanceStack[1].s_voidp = Global::modules[instance->classId.smoke].binding;
    (*fn)(0, args[0].s_voidp, initializeInstanceStack);

    return args[0].s_voidp;
}

QVariant valueToVariant(const QScriptValue& value)
{
    if (Object::Instance::isSmokeObject(value)) {
        Object::Instance * instance = Object::Instance::get(value);
        Smoke::Class & klass = instance->classId.smoke->classes[instance->classId.index];
        return QVariant(QMetaType::type(klass.className), instance->value);
    } else if (value.instanceOf(QtScriptSmoke::Global::QtEnum)) {
        return QVariant(value.property("value").toUInt32());
    } else {
        return value.toVariant();
    }
    
}

/*
    The code in this method is devived from create() in scriptengine_p.cpp in Qt 4.5.2
 */
QScriptValue valueFromVariant(QScriptEngine *engine, const QVariant& variant)
{
    QScriptValue result;
    const void * ptr = variant.data();
    int type = variant.userType();

    // check if it's one of the types we know
    switch (QMetaType::Type(type)) {
    case QMetaType::Void:
        result = engine->undefinedValue();
        break;
    case QMetaType::Bool:
        result = QScriptValue(*reinterpret_cast<const bool*>(ptr));
        break;
    case QMetaType::Int:
        result = QScriptValue(*reinterpret_cast<const int*>(ptr));
        break;
    case QMetaType::UInt:
        result = QScriptValue(*reinterpret_cast<const uint*>(ptr));
        break;
    case QMetaType::LongLong:
        result = QScriptValue(qsreal(*reinterpret_cast<const qlonglong*>(ptr)));
        break;
    case QMetaType::ULongLong:
#if defined(Q_OS_WIN) && defined(_MSC_FULL_VER) && _MSC_FULL_VER <= 12008804
#pragma message("** NOTE: You need the Visual Studio Processor Pack to compile support for 64bit unsigned integers.")
        result = QScriptValue(qsreal((qlonglong)*reinterpret_cast<const qulonglong*>(ptr)));
#elif defined(Q_CC_MSVC) && !defined(Q_CC_MSVC_NET)
        result = QScriptValue(qsreal((qlonglong)*reinterpret_cast<const qulonglong*>(ptr)));
#else
        result = QScriptValue(qsreal(*reinterpret_cast<const qulonglong*>(ptr)));
#endif
        break;
    case QMetaType::Double:
        result = QScriptValue(*reinterpret_cast<const double*>(ptr));
        break;
    case QMetaType::QString:
        result = QScriptValue(*reinterpret_cast<const QString*>(ptr));
        break;
    case QMetaType::Float:
        result = QScriptValue(*reinterpret_cast<const float*>(ptr));
        break;
    case QMetaType::Short:
        result = QScriptValue(*reinterpret_cast<const short*>(ptr));
        break;
    case QMetaType::UShort:
        result = QScriptValue(*reinterpret_cast<const unsigned short*>(ptr));
        break;
    case QMetaType::Char:
        result = QScriptValue(*reinterpret_cast<const char*>(ptr));
        break;
    case QMetaType::UChar:
        result = QScriptValue(*reinterpret_cast<const unsigned char*>(ptr));
        break;
    case QMetaType::QChar:
        result = QScriptValue((*reinterpret_cast<const QChar*>(ptr)).unicode());
        break;
    case QMetaType::QStringList:
        result = engine->toScriptValue(*reinterpret_cast<const QStringList *>(ptr));
        break;
    case QMetaType::QVariantList:
//        result = arrayFromVariantList(*reinterpret_cast<const QVariantList *>(ptr));
        break;
    case QMetaType::QVariantMap:
//        result = objectFromVariantMap(*reinterpret_cast<const QVariantMap *>(ptr));
        break;
    case QMetaType::QDateTime: 
    {
        QDateTime dateTime = *reinterpret_cast<const QDateTime *>(ptr);
        result = engine->newDate(dateTime);
    } 
    break;
    case QMetaType::QDate: 
    {
        QDate date = *reinterpret_cast<const QDate *>(ptr);
        result = engine->newDate(QDateTime(date));
    } 
    break;
    case QMetaType::QRegExp: 
    {
        QRegExp rx = *reinterpret_cast<const QRegExp *>(ptr);
        result = engine->newRegExp(rx);
    } 
    break;
    case QMetaType::QObjectStar:
    case QMetaType::QWidgetStar:
//        engine->newQObject(&result, *reinterpret_cast<QObject* const *>(ptr));
        break;
    default:
        void * value_ptr = QMetaType::construct(QMetaType::type(variant.typeName()), ptr);
        result = Global::wrapInstance(  engine, 
                                        qtcore_Smoke->findClass(variant.typeName()), 
                                        value_ptr,
                                        QScriptEngine::ScriptOwnership );
        break;
    }
    
    return result;
}

}
