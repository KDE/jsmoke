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
SmokeQtScript::mungedMethods( const QByteArray& nameFn, QScriptContext* context )
{
        QVector<QByteArray> ret;
        ret.append( nameFn );
        
        for( int i = 0; i < context->argumentCount(); i++ )
        {
            QScriptValue val = context->argument( i );
            if( val.isNumber() || val.isBool() || val.isString() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, ret) {
                    temp.append(mungedMethod + '$');
                }
                ret = temp;
            }
            else if( val.isArray() || val.isUndefined() )
            {
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, ret) {
                    temp.append(mungedMethod + '?');
                }
                ret = temp;
            }
            else if( val.isNull() )
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
                QVector<QByteArray> temp;
                foreach (QByteArray mungedMethod, ret) {
                    temp.append(mungedMethod + '#');
                }
                ret = temp;
            }
        }
        return ret;
}

void
SmokeQtScript::scriptArgumentsToSmoke( QScriptContext* context, Smoke::Stack args )
{
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
            qDebug() << "num arg" <<  args[argsPos].s_int << val.toNumber() << static_cast<int>( val.toNumber() );

        }
        else if( val.isString() )
        {
            //FIXME memory management
            args[argsPos].s_voidp = new QString(val.toString());
            qDebug() << "string arg" << val.toString();
        }
        else if( QtScript::SmokeInstance::isSmokeObject(val) )
        {
            QtScript::SmokeInstance * instance = QtScript::SmokeInstance::get(val);
            void* obj = instance->value;
            args[argsPos].s_class = obj;
            qDebug() << "attributedobject" << obj;
        }
        else 
        {
                qDebug() << "I don't know what this is." << val.toString() << val.data().toString();
                qFatal("gaaah");
        }
    }
}