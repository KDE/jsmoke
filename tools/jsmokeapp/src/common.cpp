/****************************************************************************
**
** This code is based on the 'qsexec' tool in the Qt Labs QtScript
** bindings project.
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
** Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>
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

#include <QtScript>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include <QtCore/QSet>
#include <QtDebug>

static QStringList loadPaths;

namespace {

// avoid loading files more than once
static QSet<QString> loadedFiles;

bool loadFile(QString fileName, QScriptEngine *engine)
{
    QFileInfo fileInfo(fileName);
    
    if (fileInfo.isRelative()) {
        foreach (QString loadPath, loadPaths) {
            QFileInfo candidate(loadPath + "/" + fileName);
            if (candidate.exists()) {
                fileInfo = candidate;
                break;
            }
        }
    }
    
    QString absoluteFileName = fileInfo.absoluteFilePath();
    QString absolutePath = fileInfo.absolutePath();
    QString canonicalFileName = fileInfo.canonicalFilePath();
    
    if (loadedFiles.contains(canonicalFileName)) {
        return true;
    }
    
    loadedFiles.insert(canonicalFileName);
    QString path = fileInfo.path();

    // load the file
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString contents = stream.readAll();
        file.close();

        int endlineIndex = contents.indexOf('\n');
        QString line = contents.left(endlineIndex);
        int lineNumber = 1;

        // strip off #!/usr/bin/env qscript line
        if (line.startsWith("#!")) {
            contents.remove(0, endlineIndex+1);
            ++lineNumber;
        }

        // set qt.script.absoluteFilePath
        QScriptValue script = engine->globalObject().property("qs").property("script");
        QScriptValue oldFilePathValue = script.property("absoluteFilePath");
        QScriptValue oldPathValue = script.property("absolutePath");
        script.setProperty("absoluteFilePath", engine->toScriptValue(absoluteFileName));
        script.setProperty("absolutePath", engine->toScriptValue(absolutePath));

        QScriptValue r = engine->evaluate(contents, fileName, lineNumber);
        if (engine->hasUncaughtException()) {
            QStringList backtrace = engine->uncaughtExceptionBacktrace();
            qDebug() << QString("    %1\n%2\n\n").arg(r.toString()).arg(backtrace.join("\n"));
            return true;
        }
        
        script.setProperty("absoluteFilePath", oldFilePathValue); // if we come from includeScript(), or whereever
        script.setProperty("absolutePath", oldPathValue); // if we come from includeScript(), or whereever
    } else {
        return false;
    }
    return true;
}

QScriptValue includeScript(QScriptContext *context, QScriptEngine *engine)
{
    QString currentFileName = engine->globalObject().property("qs").property("script").property("absoluteFilePath").toString();
    QFileInfo currentFileInfo(currentFileName);
    QString path = currentFileInfo.path();
    QString importFile = context->argument(0).toString();
    QFileInfo importInfo(importFile);
    if (importInfo.isRelative()) {
        importFile =  path + "/" + importInfo.filePath();
    }
    if (!loadFile(importFile, engine)) {
        return context->throwError(QString("Failed to resolve include: %1").arg(importFile));
    }
    return engine->toScriptValue(true);
}

QScriptValue importExtension(QScriptContext *context, QScriptEngine *engine)
{
    return engine->importExtension(context->argument(0).toString());
}

} // namespace

void addLoadPath(QString pathName)
{
    loadPaths << pathName;
}

QScriptEngine * initializeEngine()
{
    QScriptEngine *engine = new QScriptEngine();

    QScriptValue global = engine->globalObject();
    // add the qt object
    global.setProperty("qs", engine->newObject());
    // add a 'script' object
    QScriptValue script = engine->newObject();
    global.property("qs").setProperty("script", script);
    // add a 'system' object
    QScriptValue system = engine->newObject();
    global.property("qs").setProperty("system", system);

    // add os information to qt.system.os
#ifdef Q_OS_WIN32
    QScriptValue osName = engine->toScriptValue(QString("windows"));
#elif defined(Q_OS_LINUX)
    QScriptValue osName = engine->toScriptValue(QString("linux"));
#elif defined(Q_OS_MAC)
    QScriptValue osName = engine->toScriptValue(QString("mac"));
#elif defined(Q_OS_UNIX)
    QScriptValue osName = engine->toScriptValue(QString("unix"));
#endif
    system.setProperty("os", osName);

    // add environment variables to qt.system.env
    QMap<QString,QVariant> envMap;
    QStringList envList = QProcess::systemEnvironment();
    foreach (const QString &entry, envList) {
        QStringList keyVal = entry.split('=');
        if (keyVal.size() == 1)
            envMap.insert(keyVal.at(0), QString());
        else
            envMap.insert(keyVal.at(0), keyVal.at(1));
    }
    system.setProperty("env", engine->toScriptValue(envMap));

    // add the include functionality to qt.script.include
    script.setProperty("include", engine->newFunction(includeScript));
    // add the importExtension functionality to qt.script.importExtension
    script.setProperty("importExtension", engine->newFunction(importExtension));
    return engine;
}

int run(QScriptEngine * engine, const QString& fileName)
{
    QScriptValue global = engine->globalObject();
    QScriptValue script = global.property("qs").property("script");
    
    if (!loadFile(fileName, engine)) {
        qDebug() << "Failed:" << fileName;
        return EXIT_FAILURE;
    }

    delete engine;
    return EXIT_SUCCESS;
}
