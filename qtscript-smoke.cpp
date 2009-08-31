#include "qtscript-smoke.h"

#include <QTimer>
#include <iostream>

qtscript-smoke::qtscript-smoke()
{
    QTimer* timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), SLOT(output()) );
    timer->start( 1000 );
}

qtscript-smoke::~qtscript-smoke()
{}

void qtscript-smoke::output()
{
    std::cout << "Hello World!" << std::endl;
}

#include "qtscript-smoke.moc"
