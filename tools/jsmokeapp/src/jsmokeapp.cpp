/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtScript project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QStringList>
#include <QtGui/QApplication>
#include <QtScript/QScriptEngine>

extern QScriptEngine * initializeEngine();
extern int run(QScriptEngine * engine);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // QStringList paths = QStringList() << "/usr/lib/qt4/plugins";
    // app.setLibraryPaths(paths);
    
    QScriptEngine * engine = initializeEngine();
    engine->importExtension("jsmoke.qtcore");
    engine->importExtension("jsmoke.qtgui");
    
    return run(engine);
}