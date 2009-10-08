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

namespace QtScriptSmoke {
    
Binding::Binding(Smoke* s)
    : SmokeBinding(s) { }

char* Binding::className(Smoke::Index classId)
{
    qDebug() << "QtScriptSmoke::Binding::className " << smoke->className(classId);
    // What about '::' scope operators in className paths in JavaScript?
    return (char *) smoke->className(classId);
}

//!method called when a virtual method of a smoke-owned object is called. eg QWidget::mousePressEvent
bool Binding::callMethod(Smoke::Index method, void* ptr, Smoke::Stack args, bool isAbstract)
{
    QScriptValue * obj = QtScriptSmoke::Global::getScriptValue(ptr);
    if (obj == 0) {
        return false;
    }
    
    QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(*obj);

    if (QtScriptSmoke::Debug::DoDebug & QtScriptSmoke::Debug::Virtual) {
        Smoke::Method & meth = smoke->methods[method];
        QByteArray signature(smoke->methodNames[meth.name]);
        signature += "(";
        
        for (int i = 0; i < meth.numArgs; i++) {
            if (i != 0) {
                signature += ", ";
            }
            
            signature += smoke->types[smoke->argumentList[meth.args + i]].name;
        }
        
        signature += ")";
        
        if ((meth.flags & Smoke::mf_const) != 0) {
            signature += " const";
        }
        
        qWarning(   "module: %s virtual %p->%s::%s called", 
                    smoke->moduleName(),
                    ptr,
                    smoke->classes[smoke->methods[method].classId].className,
                    (const char *) signature );
    }

    if (instance == 0) {
        if (QtScriptSmoke::Debug::DoDebug & QtScriptSmoke::Debug::Virtual) {
            qWarning("Cannot find object for virtual method %p -> %p", ptr, obj);
        }
        
        return false;
    }
    
    // If the virtual method hasn't been overriden, return false and just call the C++ one.
    const char *methodName = smoke->methodNames[smoke->methods[method].name];
    if (obj->propertyFlags(methodName) != 0) {
        return false;
    }
    
    QScriptValue function = obj->property(methodName);   
    QtScriptSmoke::VirtualMethodCall methodCall(smoke, method, args, *obj, function);
    methodCall.next();
    return true;
}

void Binding::deleted(Smoke::Index classId, void* ptr)
{
    QScriptValue * obj = QtScriptSmoke::Global::getScriptValue(ptr);
    QtScriptSmoke::Instance * instance = QtScriptSmoke::Instance::get(*obj);
    
    if (QtScriptSmoke::Debug::DoDebug & QtScriptSmoke::Debug::GC) {
        qWarning("%p->~%s()", ptr, smoke->className(classId));
    }
    
    if (instance == 0 || instance->value == 0) {
        return;
    }
    
    QtScriptSmoke::Global::unmapPointer(instance, instance->classId.index, 0);
    instance->value = 0;
    return;
}

}
