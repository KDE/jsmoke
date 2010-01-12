/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   Based on the PerlQt marshalling code by Ashley Winters

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtWebKit/QWebDatabase>
#include <QtWebKit/QWebSecurityOrigin>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebHistory>
#include <QtWebKit/QWebPluginFactory>

#if QT_VERSION >= 0x40600
#include <QtWebKit/QWebElement>
#endif

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

#if QT_VERSION >= 0x40600
Q_DECLARE_METATYPE(QWebElement)
Q_DECLARE_METATYPE(QList<QWebElement>)
#endif

Q_DECLARE_METATYPE(QWebPluginFactory::Plugin)
Q_DECLARE_METATYPE(QList<QWebPluginFactory::Plugin>)
// Q_DECLARE_METATYPE(QWebSecurityOrigin)
// Q_DECLARE_METATYPE(QList<QWebSecurityOrigin>)
Q_DECLARE_METATYPE(QWebFrame*)
Q_DECLARE_METATYPE(QList<QWebFrame*>)
//Q_DECLARE_METATYPE(QWebHistoryItem)
//Q_DECLARE_METATYPE(QList<QWebHistoryItem>)

namespace JSmoke {
   

#if QT_VERSION >= 0x40600
#endif

Marshall::TypeHandler QtWebKitHandlers[] = {
//    { "QList<QWebDatabase>", marshall_Container<QList<QWebDatabase> > },
//    { "QList<QWebSecurityOrigin>", marshall_Container<QList<QWebSecurityOrigin> > },
    { "QList<QWebFrame*>", marshall_Container<QList<QWebFrame*> > },
    { "QList<QWebPluginFactory::Plugin>", marshall_Container<QList<QWebPluginFactory::Plugin> > },
#if QT_VERSION >= 0x40600
    { "QList<QWebElement>", marshall_Container<QList<QWebElement> > },
#endif
//    { "QList<QWebHistoryItem>", marshall_QWebHistoryItemList },
    
    { 0, 0 }
};

void registerQtWebKitTypes(QScriptEngine * engine) 
{   
    qScriptSmokeRegisterPointerSequenceMetaType<QList<QWebFrame*> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QWebPluginFactory::Plugin> >(engine); 
    
#if QT_VERSION >= 0x40600
    qScriptSmokeRegisterSequenceMetaType<QList<QWebElement> >(engine);    
#endif
//    qScriptSmokeRegisterSequenceMetaType<QList<QWebDatabase >(engine);    
//    qScriptSmokeRegisterSequenceMetaType<QList<QWebSecurityOrigin> >(engine);    
 
    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
