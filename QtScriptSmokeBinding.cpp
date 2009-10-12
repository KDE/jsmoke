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
#include "SmokeQtScriptUtils.h"

namespace QtScriptSmoke {
    
Binding::Binding(Smoke* s)
    : SmokeBinding(s) { }

char* Binding::className(Smoke::Index classId)
{
    qDebug() << "QtScriptSmoke::Binding::className " << smoke->className(classId);
    // Convert '::' to '.' heree
    return (char *) smoke->className(classId);
}

//!method called when a virtual method of a smoke-owned object is called. eg QWidget::mousePressEvent
bool Binding::callMethod(Smoke::Index method, void* ptr, Smoke::Stack args, bool isAbstract)
{
    QScriptValue * obj = Global::getScriptValue(ptr);
    if (obj == 0) {
        return false;
    }
    
    QtScriptSmoke::Instance * instance = Instance::get(*obj);

    if ((Debug::DoDebug & Debug::Virtual) != 0) {
        Smoke::ModuleIndex methodId = { smoke, methodId.index };
        
        qWarning(   "module: %s virtual %p->%s::%s called", 
                    smoke->moduleName(),
                    ptr,
                    smoke->classes[smoke->methods[method].classId].className,
                    methodToString(methodId).toLatin1().constData() );
    }

    if (instance == 0) {
        if ((Debug::DoDebug & Debug::Virtual) != 0) {
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
    VirtualMethodCall methodCall(smoke, method, args, *obj, function);
    methodCall.next();
    return true;
}

void Binding::deleted(Smoke::Index classId, void* ptr)
{
    QScriptValue * obj = Global::getScriptValue(ptr);
    Instance * instance = Instance::get(*obj);
    
    if ((Debug::DoDebug & Debug::GC) != 0) {
        qWarning("%p->~%s()", ptr, smoke->className(classId));
    }
    
    if (instance == 0 || instance->value == 0) {
        return;
    }
    
    Global::unmapPointer(instance, instance->classId.index, 0);
    instance->value = 0;
    return;
}

}
