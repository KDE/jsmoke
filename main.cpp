#include <QApplication>
#include "qtscript-smoke.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QtScriptSmoke foo;
    return app.exec();
}
