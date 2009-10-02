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

#include "QtScriptSmokeBinding.h"

#include <QtDebug>
#include <QtScript/QScriptValue>

#include "global.h"
#include "virtualmethodcall.h"

QtScriptSmokeBinding::QtScriptSmokeBinding( Smoke* s)
    : SmokeBinding( s )
{
    
}


char* QtScriptSmokeBinding::className(Smoke::Index classId)
{
    qDebug() << "QtScriptSmokeBinding::className" << classId;
    return (char *) smoke->classes[classId].className;
}

//!method called when a virtual method of a smoke-owned object is called. eg QWidget::mousePressEvent
bool QtScriptSmokeBinding::callMethod(Smoke::Index method, void* ptr, Smoke::Stack args, bool isAbstract)
{
    //TODO: we're going to have to keep a hash<void*, QScriptValue> and then query
    //the scriptvalue to see if the user has overridden a virtual method
    QScriptValue * obj = QtScript::Global::getScriptValue(ptr);
    QtScript::SmokeInstance * instance = QtScript::SmokeInstance::get(*obj);

    if (QtScript::Debug::DoDebug & QtScript::Debug::Virtual) {
        Smoke::Method & meth = smoke->methods[method];
        QByteArray signature(smoke->methodNames[meth.name]);
        signature += "(";
            for (int i = 0; i < meth.numArgs; i++) {
        if (i != 0) signature += ", ";
            signature += smoke->types[smoke->argumentList[meth.args + i]].name;
        }
        signature += ")";
        if (meth.flags & Smoke::mf_const) {
            signature += " const";
        }
        qWarning(   "module: %s virtual %p->%s::%s called", 
                    smoke->moduleName(),
                    ptr,
                    smoke->classes[smoke->methods[method].classId].className,
                    (const char *) signature );
    }

    if (instance == 0) {
        if (QtScript::Debug::DoDebug & QtScript::Debug::Virtual)   // if not in global destruction
            qWarning("Cannot find object for virtual method %p -> %p", ptr, &obj);
        return false;
    }
    const char *methodName = smoke->methodNames[smoke->methods[method].name];
    
    // If the virtual method hasn't been overriden, just call the C++ one.
    // So how can we tell whether or not the method has been overriden in QtScript?
    return false;

    QScriptValue overridenMethod = obj->property(methodName);    
    QtScript::VirtualMethodCall call(smoke, method, args, *obj, overridenMethod);
    call.next();
    return true;
}

void QtScriptSmokeBinding::deleted(Smoke::Index classId, void* obj)
{
    //TODO: memory management of any sort :D
    qDebug() << "QtScriptSmokeBinding::deleted" << classId << obj;
    return;
}

