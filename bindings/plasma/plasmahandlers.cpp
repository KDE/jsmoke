/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

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

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

#include <plasma/abstractrunner.h>
#include <plasma/applet.h>
#include <plasma/extendergroup.h>
#include <plasma/widgets/signalplotter.h>

/*
#include <plasma/abstracttoolbox.h>
#include <plasma/containment.h>
#include <plasma/containmentactions.h>
#include <plasma/containmentactionspluginsconfig.h>
#include <plasma/extender.h>
#include <plasma/extenderitem.h>
#include <plasma/glapplet.h>
#include <plasma/querymatch.h>
#include <plasma/runnersyntax.h>
#include <plasma/scripting/appletscript.h>
#include <plasma/widgets/busywidget.h>
#include <plasma/widgets/flashinglabel.h>
#include <plasma/widgets/frame.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/itembackground.h>
#include <plasma/widgets/meter.h>
#include <plasma/widgets/scrollwidget.h>
#include <plasma/widgets/separator.h>
#include <plasma/widgets/svgwidget.h>
#include <plasma/widgets/tabbar.h>
#include <plasma/widgets/webview.h>
*/

Q_DECLARE_METATYPE(Plasma::AbstractRunner*)
Q_DECLARE_METATYPE(Plasma::Applet*)
Q_DECLARE_METATYPE(Plasma::Containment*)
Q_DECLARE_METATYPE(Plasma::ExtenderGroup*)
Q_DECLARE_METATYPE(Plasma::ExtenderItem*)
Q_DECLARE_METATYPE(Plasma::Location)
Q_DECLARE_METATYPE(Plasma::PlotColor)
Q_DECLARE_METATYPE(QList<Plasma::AbstractRunner*>)
Q_DECLARE_METATYPE(QList<Plasma::Applet*>)
Q_DECLARE_METATYPE(QList<Plasma::Containment*>)
Q_DECLARE_METATYPE(QList<Plasma::ExtenderGroup*>)
Q_DECLARE_METATYPE(QList<Plasma::ExtenderItem*>)
Q_DECLARE_METATYPE(QList<Plasma::Location>)
Q_DECLARE_METATYPE(QList<Plasma::PlotColor>)

namespace JSmoke {

DEF_CONTAINER_MARSHALLER(QListPlasmaAbstractRunner, QList<Plasma::AbstractRunner*>)
DEF_CONTAINER_MARSHALLER(QListPlasmaApplet, QList<Plasma::Applet*>)
DEF_CONTAINER_MARSHALLER(QListPlasmaContainment, QList<Plasma::Containment*>)
DEF_CONTAINER_MARSHALLER(QListPlasmaExtenderGroup, QList<Plasma::ExtenderGroup*>)
DEF_CONTAINER_MARSHALLER(QListPlasmaExtenderItem, QList<Plasma::ExtenderItem*>)
DEF_CONTAINER_MARSHALLER(QListPlasmaLocation, QList<Plasma::Location>)
DEF_CONTAINER_MARSHALLER(QListPlasmaPlotColor, QList<Plasma::PlotColor>)

Marshall::TypeHandler PlasmaHandlers[] = {
    { "QList<Plasma::AbstractRunner*>", marshall_QListPlasmaAbstractRunner },
    { "QList<Plasma::Applet*>", marshall_QListPlasmaApplet },
    { "QList<Plasma::Containment*>", marshall_QListPlasmaContainment },
    { "QList<Plasma::ExtenderGroup*>", marshall_QListPlasmaExtenderGroup },
    { "QList<Plasma::ExtenderItem*>", marshall_QListPlasmaExtenderItem },
    { "QList<Plasma::Location>", marshall_QListPlasmaLocation },
    { "QList<Plasma::PlotColor>&", marshall_QListPlasmaPlotColor },
    { 0, 0 }
};

void registerPlasmaTypes(QScriptEngine * engine)
{
    qScriptSmokeRegisterSequenceMetaType<QList<Plasma::Location> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<Plasma::PlotColor> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<Plasma::AbstractRunner*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Plasma::Applet*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Plasma::Containment*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Plasma::ExtenderGroup*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<Plasma::ExtenderItem*> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
