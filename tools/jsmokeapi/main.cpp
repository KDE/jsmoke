/*
    Command line introspection tool for SMOKE libs
    Copyright (C) 2010 Arno Rehn <arno@arnorehn.de>
    Copyright (C) 2010 Richard Dale <richard.j.dale@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QtCore>
#include <QtDebug>

#include <smoke.h>
#include <smoke/qtcore_smoke.h>

typedef void (*InitSmokeFn)();
static QList<Smoke*> smokeModules;

static bool showParents;
static bool matchPattern;
static bool caseInsensitive;
static QRegExp targetPattern;

static Smoke* 
loadSmokeModule(QString moduleName) {
    QFileInfo file(QString("libsmoke") + moduleName);
    QLibrary lib(file.filePath());

    if (moduleName == "qtcore") {
        init_qtcore_Smoke();
        return qtcore_Smoke;
    }

    QString init_name = "init_" + moduleName + "_Smoke";
    InitSmokeFn init = (InitSmokeFn) lib.resolve(init_name.toLatin1());

    if (!init)
        qFatal("Couldn't resolve %s: %s", qPrintable(init_name), qPrintable(lib.errorString()));
    
    (*init)();

    QString smoke_name = moduleName + "_Smoke";
    Smoke** smoke = (Smoke**) lib.resolve(smoke_name.toLatin1());
    if (!smoke)
        qFatal("Couldn't resolve %s: %s", qPrintable(smoke_name), qPrintable(lib.errorString()));

    return *smoke;
}

static QString
argToJavaScript(QString arg)
{
    arg.replace(QLatin1String("QStringList"), QLatin1String("[String, ..]"));
    arg.replace(QLatin1String("void*"), QLatin1String("undefined"));
    arg.replace(QLatin1String("::"), QLatin1String("."));
    arg.replace(QLatin1String("char*"), QLatin1String("String"));
    arg.replace(QLatin1String("char**"), QLatin1String("[String, ..]"));
    arg.replace(QLatin1String("&"), QLatin1String(""));
    arg.replace(QLatin1String("*"), QLatin1String(""));
    arg.replace(QLatin1String("const "), QLatin1String(""));
    arg.replace(QLatin1String("void "), QLatin1String(""));
    arg.replace(QLatin1String("static "), QLatin1String(""));
    arg.replace(QLatin1String(" const"), QLatin1String(""));
    arg.replace(QLatin1String("unsigned short"), QLatin1String("Number"));
    arg.replace(QLatin1String("short"), QLatin1String("Number"));
    arg.replace(QLatin1String("unsigned int"), QLatin1String("Number"));
    arg.replace(QRegExp("^int$"), QLatin1String("Number"));
    arg.replace(QLatin1String("unsigned long"), QLatin1String("Number"));
    arg.replace(QLatin1String("long"), QLatin1String("Number"));
    arg.replace(QLatin1String("double"), QLatin1String("Number"));
    arg.replace(QLatin1String("real"), QLatin1String("Number"));
    arg.replace(QLatin1String("bool"), QLatin1String("Boolean"));
    arg.replace(QLatin1String("QString"), QLatin1String("String"));
    arg.replace(QRegExp("(QList|QVector)<QPair<([^>])>,<([^>]*)>>"), QLatin1String("[[\\2,\\3], ..]"));
    arg.replace(QRegExp("(QList|QVector)<([^>]*)>"), QLatin1String("[\\2, ..]"));
    arg.replace(QRegExp("(QHash|QMap)<([^,]*),([^>]*)>"), QLatin1String("{\\2:\\3, ..}"));

    return arg;
}

static QString
methodToString(Smoke::ModuleIndex methodId)
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

    QString typeName = QString::fromLatin1(smoke->types[methodRef.ret].name);
    
    QString className(smoke->classes[methodRef.classId].className);
    className.replace("::", ".");
    
    if ((methodRef.flags & Smoke::mf_enum) != 0) {
        result.append(QString("%1.%2")
                            .arg(className)
                            .arg(smoke->methodNames[methodRef.name]) );
        return result;
    }
    
    if ((methodRef.flags & Smoke::mf_static) != 0) {
//        result.append("static ");
    }
    
    if (    !typeName.isNull() 
            && typeName != QLatin1String("void")
            && (methodRef.flags & Smoke::mf_ctor) == 0 ) 
    {
        result.append(argToJavaScript(typeName) + " ");
    }
    
    result.append(  QString("%1.%2(")
                        .arg(className)
                        .arg(smoke->methodNames[methodRef.name]) );
                        
    for (int i = 0; i < methodRef.numArgs; i++) {
        if (i > 0) {
            result.append(", ");
        }
        
        typeName = QString::fromLatin1(smoke->types[smoke->argumentList[methodRef.args+i]].name);
        if (!typeName.isNull()) {
            if ((methodRef.flags & Smoke::mf_signal) != 0) {
                result.append(typeName);
            } else {
                result.append(argToJavaScript(typeName));
            }
        }
    }
    
    result.append(")");
    if ((methodRef.flags & Smoke::mf_const) != 0) {
//        result.append(" const");
    }
    
    return result;
}

static QList<Smoke::ModuleIndex>
getAllParents(const Smoke::ModuleIndex& classId)
{
    Smoke* smoke = classId.smoke;
    QList<Smoke::ModuleIndex> result;
           
    for (   Smoke::Index * parent = smoke->inheritanceList + smoke->classes[classId.index].parents; 
            *parent != 0; 
            parent++ ) 
    {
        Smoke::ModuleIndex parentId = Smoke::findClass(smoke->classes[*parent].className);
        Q_ASSERT(parentId != Smoke::NullModuleIndex);
        result << getAllParents(parentId);
    }
    
    result << classId;
    return result;
}

static void
listAllMethods(const Smoke::ModuleIndex& classId)
{
    Smoke * smoke = classId.smoke;
    Smoke::Index imax = smoke->numMethodMaps;
    Smoke::Index imin = 0, icur = -1, methmin, methmax;
    methmin = -1; methmax = -1; // kill warnings
    int icmp = -1;

    while (imax >= imin) {
        icur = (imin + imax) / 2;
        icmp = smoke->leg(smoke->methodMaps[icur].classId, classId.index);
        if (icmp == 0) {
            Smoke::Index pos = icur;
            while (icur != 0 && smoke->methodMaps[icur-1].classId == classId.index) {
                icur --;
            }
            
            methmin = icur;
            icur = pos;
            while (icur < imax && smoke->methodMaps[icur+1].classId == classId.index) {
                icur ++;
            }
            
            methmax = icur;
            break;
        }
        
        if (icmp > 0) {
            imax = icur - 1;
        } else {
            imin = icur + 1;
        }
    }

    if (icmp == 0) {
        for (Smoke::Index i = methmin ; i <= methmax ; i++) {
            Smoke::Index ix = smoke->methodMaps[i].method;
            if (ix >= 0) {  // single match
                if ((smoke->methods[ix].flags & Smoke::mf_dtor) == 0) {
                    QString method = methodToString(Smoke::ModuleIndex(smoke, ix));
                    if (!matchPattern || targetPattern.indexIn(method) != -1) {
                        qWarning("%s", method.toLatin1().constData());
                    }
                }
            } else {        // multiple match
                ix = -ix;       // turn into ambiguousMethodList index
                while (smoke->ambiguousMethodList[ix]) {
                    if ((smoke->methods[smoke->ambiguousMethodList[ix]].flags & Smoke::mf_dtor) == 0) {
                        QString method = methodToString(Smoke::ModuleIndex(smoke, smoke->ambiguousMethodList[ix]));
                        if (!matchPattern || targetPattern.indexIn(method) != -1) {
                            qWarning("%s", method.toLatin1().constData());
                        }
                    }
                    
                    ix++;
                }
            }
        }
    }
}

#define PRINT_USAGE() \
    qDebug() << "Usage:" << argv[0] << "-r <smoke lib> [-r more smoke libs..] [-p] [-m pattern] [-i] [<classname(s)>..]"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QStringList arguments = app.arguments();
    
    showParents = false;
    caseInsensitive = false;
    matchPattern = false;

    if (argc == 1) {
        PRINT_USAGE();
        return 0;
    }

    int i = 1;
    while (i < arguments.length()) {
        if (arguments[i] == QLatin1String("-h") || arguments[i] == QLatin1String("--help")) {
            PRINT_USAGE();
            return 0;
        } else if (arguments[i] == QLatin1String("-r") || arguments[i] == QLatin1String("--require")) {
            i++;
            if (i < arguments.length()) {
                smokeModules << loadSmokeModule(arguments[i]);
            }
            i++;
        } else if (arguments[i] == QLatin1String("-p") || arguments[i] == QLatin1String("--parents")) {
            showParents = true;
            i++;
        } else if (arguments[i] == QLatin1String("-i") || arguments[i] == QLatin1String("--insensitive")) {
            caseInsensitive = true;
            i++;
        } else if (arguments[i] == QLatin1String("-m") || arguments[i] == QLatin1String("--match")) {
            i++;
            if (i < arguments.length()) {
                targetPattern = QRegExp(arguments[i]);
                matchPattern = true;
            }
            i++;
        } else {
            break;
        }        
    }

    if (caseInsensitive) {
        targetPattern.setCaseSensitivity(Qt::CaseInsensitive);
    }
    
    smokeModules << loadSmokeModule("qtcore");
    
    if (i >= arguments.length()) {
        if (targetPattern.isEmpty()) {
            PRINT_USAGE();
            return 0;
        } else {
            foreach (Smoke * smoke, smokeModules) {
                for (int i = 1; i <= smoke->numClasses; i++) {
                    if (!smoke->classes[i].external) {
                        listAllMethods(Smoke::ModuleIndex(smoke, i));
                    }
                }
            }
            
            return 0;
        }
    }
    
    while (i < arguments.length()) {
        QString className = arguments[i];
        className.replace(".", "::");
        
        Smoke::ModuleIndex classId = Smoke::findClass(className.toLatin1());
        if (classId == Smoke::NullModuleIndex) {
            qFatal("Error: class '%s' not found", className.toLatin1().constData());
        }
        
        if (showParents) {
            QList<Smoke::ModuleIndex> parents = getAllParents(classId);
            foreach (Smoke::ModuleIndex parentId, parents) {
                listAllMethods(parentId);
            }
        } else {
            listAllMethods(classId);
        }
        
        i++;
    }
    
    return 0;
}
