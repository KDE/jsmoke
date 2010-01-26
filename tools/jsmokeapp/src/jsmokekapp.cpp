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
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include <KStandardDirs>
#include <KComponentData>
#include <kdebug.h>
#include <kcmdlineargs.h>
#include <kapplication.h>
#include <kaboutdata.h>

extern void addLoadPath(QString loadPath);
extern QScriptEngine * initializeEngine();
extern int run(QScriptEngine * engine, const QString& fileName);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        qFatal("Usage: %s application_directory/qs_script [Qt-options] [KDE-options]\n", argv[0]);
    }

    QFileInfo script(argv[1]);       
    QString path(argv[1]);
    QByteArray programName("jsmokekapp");
    
    if (script.isRelative() && path.contains(QDir::separator())) {
        programName = script.dir().dirName().toLatin1();
        KComponentData componentData(programName, QByteArray(), KComponentData::SkipMainComponentRegistration);
        path = componentData.dirs()->locate("data", argv[1], componentData);
        script = QFileInfo(path);
        addLoadPath(script.path());
    }
    
    if (!script.exists()) {
        qFatal("JavaScript %s missing", argv[1]);
    }
    
    char ** qsargs = (char **) calloc(argc - 1, sizeof(char *));
    for (int i = 0; i < (argc - 1); i++) {
        qsargs[i] = strdup(argv[i]);
    }

    KAboutData * about = new KAboutData(programName, "QtScript KDE App", ki18n(""), "0.1");
    KCmdLineArgs::init(argc, qsargs, about);
    KApplication app;
    
    QScriptEngine * engine = initializeEngine();
    engine->importExtension("jsmoke.qtcore");
    engine->importExtension("jsmoke.qtgui");
    engine->importExtension("jsmoke.kdecore");
    engine->importExtension("jsmoke.kdegui");
    
    run(engine, QFile::encodeName(script.filePath()));
    return app.exec();
}