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
#include "metaclass.h"

namespace JSmoke {

QString
methodToString(const Smoke::ModuleIndex& methodId)
{
    QString result;
    Smoke * smoke = methodId.smoke;
    Smoke::Method& methodRef = smoke->methods[methodId.index];
    
    if ((methodRef.flags & Smoke::mf_signal) != 0) {
        result.append("signal ");
    }
    
    if ((methodRef.flags & Smoke::mf_slot) != 0) {
        result.append("slot ");
    }
    
    const char * typeName = smoke->types[methodRef.ret].name;
    
    if ((methodRef.flags & Smoke::mf_enum) != 0) {
        result.append(QString("enum %1::%2")
                            .arg(smoke->classes[methodRef.classId].className)
                            .arg(smoke->methodNames[methodRef.name]) );
        return result;
    }
    
    if ((methodRef.flags & Smoke::mf_virtual) != 0) {
        result.append("virtual ");
    }
    
    if ((methodRef.flags & Smoke::mf_static) != 0) {
        result.append("static ");
    }
    
    if ((methodRef.flags & Smoke::mf_ctor) == 0) {
        result.append((typeName != 0 ? typeName : "void"));
        result.append(" ");
    }
    
    result.append(  QString("%1::%2(")
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
    
    if ((methodRef.flags & Smoke::mf_purevirtual) != 0) {
        result.append(" = 0");
    }
    
    return result;
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

QByteArray 
constructorName(const Smoke::ModuleIndex& classId)
{
    QByteArray name(classId.smoke->classes[classId.index].className);
    int pos = name.lastIndexOf("::");
    
    if (pos != -1) {
        name = name.mid(pos + strlen("::"));
    }
    
    return name;
}

static QByteArray
typeName(const Smoke::Type& typeRef)
{
    QByteArray name(typeRef.name);
    return name.replace("const ", "").replace("&", "").replace("*", "");
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
static QVector<QByteArray> 
mungedMethods(const QByteArray& methodName, const QScriptValueList& args, MethodMatchesState matchState)
{
    QVector<QByteArray> result;
    result.append(methodName);
    
    foreach (QScriptValue value, args) {
        if (    value.isNumber() 
                || value.isBool() 
                || value.isString() 
                || value.instanceOf(JSmoke::Global::QtEnum) )
        {
            if (matchState == ImplicitTypeConversionsState) {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, result) {
                    temp.append(mungedMethod + '$');
                    temp.append(mungedMethod + '#');
                }
                result = temp;
            } else {
                for (int i = 0; i < result.count(); i++) {
                    result[i] += '$';
                }
            }
        } else if (value.isArray()) {
            if (matchState == ImplicitTypeConversionsState) {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, result) {
                    temp.append(mungedMethod + '?');
                    temp.append(mungedMethod + '#');
                }
                result = temp;
            } else {
                for (int i = 0; i < result.count(); i++) {
                    result[i] += '?';
                }
            }
        } else if (value.isNull() || value.isUndefined()) {
            QVector<QByteArray> temp;
            foreach (QByteArray mungedMethod, result) {
                temp.append(mungedMethod + '$');
                temp.append(mungedMethod + '?');
                temp.append(mungedMethod + '#');
            }
            result = temp;
        } else if ( Object::Instance::isSmokeObject(value) 
                    || value.isDate()
                    || value.isRegExp() )
        {
            for (int i = 0; i < result.count(); i++) {
                result[i] += '#';
            }
        } else {
            qDebug() << "Unknown value type:" << value.toString();
        }
    }
    
    return result;
}

static int
inheritanceDistance(Smoke* smoke, Smoke::Index classId, Smoke::Index baseId, int distance)
{
    if (baseId == 0) {
        return 100;
    }
    
    if (classId == baseId) {
        return distance;
    }
    
    for (   Smoke::Index* parent = smoke->inheritanceList + smoke->classes[classId].parents; 
            *parent != 0; 
            parent++ ) 
    {
        int result = inheritanceDistance(smoke, *parent, baseId, distance + 1);
        if (result != 100) {
            return result;
        }
    }
    
    return 100;
}

// The code to match method arguments here, is based on callQtMethod() 
// in script/qscriptextqobject.cpp
//
static int 
matchArgument(const QScriptValue& actual, const Smoke::Type& typeRef)
{
    QByteArray fullArgType(typeRef.name);
    fullArgType.replace("const ", "");
    QByteArray argType(typeName(typeRef));    
    int matchDistance = 0;
    
    if (actual.isNumber()) {
        switch (typeRef.flags & Smoke::tf_elem) {
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
        {
            if (QString::fromLatin1(fullArgType).contains(QRegExp("^(signed|unsigned)?(bool|char|short|int|long|double)[&*]$")))
                matchDistance += 7;
            else
                matchDistance += 100;
            break;
        }
        }
    } else if (actual.instanceOf(JSmoke::Global::QtEnum)) {
        switch (typeRef.flags & Smoke::tf_elem) {
        case Smoke::t_enum:
            if (actual.property("typeName").toString().toLatin1() != argType) {
                matchDistance += 100;
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
            matchDistance += 100;
            break;
        }
    } else if (actual.isString()) {
        if (argType == "QString") {
        } else if (fullArgType == "char*" || fullArgType == "unsigned char*") {
            matchDistance += 1;
        } else if (argType == "QChar") {
            matchDistance += 2;
        } else {
            matchDistance += 100;
        }
    } else if (actual.isBool()) {
        if ((typeRef.flags & Smoke::tf_elem) == Smoke::t_bool) {
        } else {
            matchDistance += 100;
        }
    } else if (actual.isDate()) {
        if (argType == "QDataTime") {
        } else if (argType == "QDate") {
            matchDistance += 1;
        } else if (argType == "QTime") {
            matchDistance += 2;
        } else {
            matchDistance += 100;
        }
    } else if (actual.isRegExp()) {
        if (argType == "QRegExp") {
        } else {
            matchDistance += 100;
        }
    } else if (Object::Instance::isSmokeObject(actual)) {
        if ((typeRef.flags & Smoke::t_class) != 0) {
            Object::Instance * instance = Object::Instance::get(actual);
            Smoke::ModuleIndex classId = instance->classId.smoke->idClass(argType, true);
            matchDistance += inheritanceDistance(instance->classId.smoke, instance->classId.index, classId.index, 0);
        } else {
            matchDistance += 100;
        }
    } else if (actual.isVariant()) {
    } else if (actual.isArray()) {
        if (    argType.contains("QVector") 
                || argType.contains("QList")
                || argType.contains("QStringList")
                || fullArgType.contains("char**")
                || QString::fromLatin1(fullArgType).contains(QRegExp("^(signed|unsigned)?(bool|char|short|int|long|double)[&*]$")) )
        {
        } else {
            matchDistance += 100;
        }
    } else if (actual.isQObject()) {
    } else if (actual.isNull()) {
    } else if (actual.isObject()) {
    } else {
        matchDistance += 100;
    }
    
    return matchDistance;
}

static QVector<Smoke::ModuleIndex>
findCandidates(Smoke::ModuleIndex classId, const QVector<QByteArray>& mungedMethods)
{
    Smoke::Class & klass = classId.smoke->classes[classId.index];
    QVector<Smoke::ModuleIndex> methodIds;

    foreach (QByteArray mungedMethod, mungedMethods) {
        Smoke::ModuleIndex methodId = classId.smoke->findMethod(klass.className, mungedMethod);

        if (methodId.index == 0) {
            // Look in the global namespaces of all the open smoke modules
            QHashIterator<Smoke*, Module> it(Global::modules);
            while (it.hasNext()) {
                it.next();
                Smoke * smoke = it.key();
                methodId = smoke->findMethod(   smoke->idClass("QGlobalSpace"), 
                                                smoke->idMethodName(mungedMethod) );
                
                if (methodId != Smoke::NullModuleIndex) {
                    methodIds.append(methodId);
                }
            }
        } else {
            methodIds.append(methodId);
        }
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
    
    return candidates;
}

MethodMatches 
resolveMethod(  Smoke::ModuleIndex classId, 
                const QByteArray& methodName, 
                QScriptContext* context, 
                const QScriptValueList& args, 
                MethodMatchesState matchState )
{
    QVector<QByteArray> mungedMethods = JSmoke::mungedMethods(methodName, args, matchState);
    QVector<Smoke::ModuleIndex> candidates = findCandidates(classId, mungedMethods);
    MethodMatches matches;
        
    foreach (Smoke::ModuleIndex method, candidates) {
        Smoke::Method& methodRef = method.smoke->methods[method.index];
        
        if ((methodRef.flags & Smoke::mf_internal) == 0) {
            QVector<Smoke::ModuleIndex> methods;
            methods.append(method);
            int matchDistance = 0;
            
            // If a method is overloaded only on const-ness, prefer the
            // non-const version
            if ((methodRef.flags & Smoke::mf_const) != 0) {
                matchDistance += 1;
            }
            
            for (int i = 0; i < methodRef.numArgs; i++) {
                QScriptValue actual = args[i];
                ushort argFlags = method.smoke->types[method.smoke->argumentList[methodRef.args+i]].flags;
                int distance = matchArgument(actual, method.smoke->types[method.smoke->argumentList[methodRef.args+i]]);
                
                if (matchState == ImplicitTypeConversionsState) {
                    if ((argFlags & Smoke::tf_elem) == Smoke::t_class && distance >= 100) {
                        QByteArray className = typeName(method.smoke->types[method.smoke->argumentList[methodRef.args+i]]);
                        Smoke::ModuleIndex argClassId = Smoke::findClass(className);
                        MethodMatches cmatches = resolveMethod( argClassId, 
                                                                constructorName(argClassId), 
                                                                context, 
                                                                QScriptValueList() << actual,
                                                                ArgumentTypeConversionState );
                        
                        if (cmatches.count() > 0 && cmatches[0].second <= 10) {
                            matchDistance += cmatches[0].second;
                            methods.append(cmatches[0].first[0]);
                            continue;
                        }
                        
                        if (Object::Instance::isSmokeObject(actual)) {
                            Object::Instance * instance = Object::Instance::get(actual);
                            cmatches = resolveMethod(   instance->classId, 
                                                        QByteArray("operator ") + className, 
                                                        context, 
                                                        QScriptValueList(),
                                                        ArgumentTypeConversionState );
                            
                            if (cmatches.count() > 0 && cmatches[0].second <= 10) {
                                matchDistance += cmatches[0].second;
                                methods.append(cmatches[0].first[0]);
                                continue;
                            }
                        }
                    }
                    
                    methods.append(Smoke::NullModuleIndex);
                }
                
                matchDistance += distance;
            }
            
            if (matches.count() > 0 && matchDistance <= matches[0].second) {
                matches.prepend(MethodMatch(methods, matchDistance));
            } else {
                matches.append(MethodMatch(methods, matchDistance));
            }
        }
    }

    if (    matchState == InitialState
            && (matches.count() == 0 || matches[0].second >= 100) )
    {
        // Failed to find a matching method, so try again assuming implicit type 
        // conversions can be done for the arguments
        matches = resolveMethod(classId, methodName, context, args, ImplicitTypeConversionsState);
    }
    
    if (matchState != ImplicitTypeConversionsState && (Debug::DoDebug & Debug::MethodMatches) != 0) {
        QStringList argsString;
        foreach (QScriptValue arg, args) {
            argsString << arg.toString();
        }
        Smoke::Class& klass = classId.smoke->classes[classId.index];
        QScriptContextInfo contextInfo(context->parentContext());
        qWarning(   "%s@%s:%d for %s.%s(%s):",
                    matchState == InitialState ? "Method matches" : "    Argument type conversion matches",
                    contextInfo.fileName().toLatin1().constData(),
                    contextInfo.lineNumber(),
                    klass.className, 
                    methodName.constData(),
                    argsString.join(", ").toLatin1().constData() );
        for (int i = 0; i < matches.count(); i++) {
            qWarning("%s%s module: %s index: %d matchDistance: %d", 
                matchState == InitialState ? "    " : "        ",
                methodToString(matches[i].first[0]).toLatin1().constData(),
                matches[i].first[0].smoke->moduleName(), 
                matches[i].first[0].index, 
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
    Smoke::ModuleIndex classId = static_cast<MetaClass*>(cls)->classId();
    
    QScriptValueList args;
    for (int count = 0; count < context->argumentCount(); count++) {
        args << context->argument(count);
    }
    
    MethodMatches matches = JSmoke::resolveMethod(classId, nameFn.toLatin1(), context, args);
    
    if (matches.count() == 0 || matches[0].second >= 100) {
        QString message = QString("%1 is not defined").arg(nameFn);
        return context->throwError(QScriptContext::ReferenceError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        // Error if the first two have the same matchDistance
        QString message = QString("overloaded %1() call not resolved").arg(nameFn);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }
    
    JSmoke::MethodCall methodCall(matches[0].first, context, engine, args);
    methodCall.next();
    return *(methodCall.var());
}

QScriptValue 
callSmokeMethod(QScriptContext* context, QScriptEngine* engine)
{
    QString nameFn = context->callee().data().toString();
    Object::Instance * instance = Object::Instance::get(context->thisObject());
    
    QScriptValueList args;
    for (int count = 0; count < context->argumentCount(); count++) {
        args << context->argument(count);
    }
    
    MethodMatches matches = JSmoke::resolveMethod(instance->classId, nameFn.toLatin1(), context, args);
    
    if (matches.count() == 0 || matches[0].second >= 100) {
        QString message = QString("%1 is not defined").arg(nameFn);
        return context->throwError(QScriptContext::ReferenceError, message);
    } else if (matches.count() > 1 && matches[0].second == matches[1].second) {
        // Error if the first two have the same matchDistance
        QString message = QString("overloaded %1() call not resolved").arg(nameFn);
        return context->throwError(QScriptContext::TypeError, message);
    } else {
        // Good, found a single best match in matches[0]
    }

    JSmoke::MethodCall methodCall(matches[0].first, context, engine, args);
    methodCall.next();
    return *(methodCall.var());
}

void *
constructCopy(Object::Instance* instance)
{
    Smoke * smoke = instance->classId.smoke;
    const char *className = smoke->className(instance->classId.index);    
    QByteArray ccSignature = constructorName(instance->classId);
    ccSignature.append("#");    
    QByteArray ccArg("const ");
    ccArg.append(className).append("&");
    Smoke::ModuleIndex ccId = smoke->findMethodName(className, ccSignature);
    Smoke::ModuleIndex ccMethod = smoke->findMethod(instance->classId, ccId);

    if (ccMethod.index == 0) {
        qWarning("JSmoke::constructCopy() failed %s %p\n", className, instance->value);
        return 0;
    }
    
    Smoke::Index method = ccMethod.smoke->methodMaps[ccMethod.index].method;
    if (method > 0) {
        // Make sure it's a copy constructor
        if (ccArg != smoke->types[*(smoke->argumentList + smoke->methods[method].args)].name) {
            qWarning("JSmoke::constructCopy() failed %s %p\n", className, instance->value);
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
            qWarning("JSmoke::constructCopy() failed %s %p\n", className, instance->value);
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
    } else if (value.instanceOf(JSmoke::Global::QtEnum)) {
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
                                        Smoke::findClass(variant.typeName()), 
                                        value_ptr,
                                        QScriptEngine::ScriptOwnership );
        break;
    }
    
    return result;
}

}
