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

#include "qtscript-smoke.h"

#include "marshall.h"
#include "ImplementationClass.h"
#include "StaticClass.h"
#include "QtScriptSmokeBinding.h"

#include "smoke/qt_smoke.h"

#include <QFile>
#include <QScriptEngine>
#include <QStringList>
#include <QtDebug>
#include <QTimer>

ImplementationClass* QtScriptSmoke::s_implClass = 0;

QtScriptSmoke::QtScriptSmoke()
{
    init_qt_Smoke();
    QtScript::installHandlers(QtScript::Handlers);

    QTimer::singleShot( 0, this, SLOT( output() ) );
}

QtScriptSmoke::~QtScriptSmoke()
{}

QScriptValue 
QtScriptSmoke::includeQtClass(QScriptContext *context, QScriptEngine* engine) //STATIC
{
    if( context->argumentCount() == 1 && context->argument(0).isString() )
    {
        QByteArray className( context->argument(0).toString().toLatin1() );
        if( qt_Smoke->findClass(className).index != 0 )
        {
            QScriptClass* sclass = new StaticClass( engine, className, s_implClass );
            QScriptValue classValue = engine->newObject( sclass );
            engine->globalObject().setProperty(context->argument(0).toString(), classValue );
        }
        else
            context->throwError(QScriptContext::RangeError, className + " is not a supported class." );
    }
    else
       context->throwError(QScriptContext::TypeError, "Only one string argument for include.");

    return QScriptValue();
}

void
QtScriptSmoke::output()
{
    QScriptEngine* engine = new QScriptEngine( this );
    s_implClass = new ImplementationClass( engine );
    QScriptValue includeFn = engine->newFunction( QtScriptSmoke::includeQtClass, 1 );
    engine->globalObject().setProperty( "include", includeFn );
    
    qDebug() << "opening ../test.js";
    QFile testFile("../test.js");
    testFile.open( QFile::ReadOnly );
    QByteArray code = testFile.readAll();
    
    engine->evaluate( code, "test.js" );
    qDebug() << engine->isEvaluating();
    qDebug() << engine->hasUncaughtException();
    qDebug() << engine->uncaughtExceptionBacktrace();

}

#include "qtscript-smoke.moc"
