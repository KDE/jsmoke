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

#include <QtCore/QByteArray>                                                                                                              
#include <QtCore/QVariant>
#include <QtCore/QEvent>

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

#include <global.h>
#include <marshall.h>
#include <marshallmacros.h>
#include <utils.h>
#include <object.h>

#include <smoke/qtcore_smoke.h>

static void
qeventTypeResolver(JSmoke::Object::Instance * instance)
{
    Smoke * smoke = instance->classId.smoke;    
    QEvent * qevent = static_cast<QEvent*>( smoke->cast(    instance->value, 
                                                            instance->classId,
                                                            JSmoke::Global::QEventClassId ) );
    switch (qevent->type()) {
    case QEvent::Timer:
        instance->classId = smoke->findClass("QTimerEvent");
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
        instance->classId = smoke->findClass("QMouseEvent");
        break;
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
        instance->classId = smoke->findClass("QKeyEvent");
        break;
    case QEvent::FocusIn:
    case QEvent::FocusOut:
        instance->classId = smoke->findClass("QFocusEvent");
        break;
    case QEvent::Enter:
    case QEvent::Leave:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::Paint:
        instance->classId = smoke->findClass("QPaintEvent");
        break;
    case QEvent::Move:
        instance->classId = smoke->findClass("QMoveEvent");
        break;
    case QEvent::Resize:
        instance->classId = smoke->findClass("QResizeEvent");
        break;
    case QEvent::Create:
    case QEvent::Destroy:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::Show:
        instance->classId = smoke->findClass("QShowEvent");
        break;
    case QEvent::Hide:
        instance->classId = smoke->findClass("QHideEvent");
    case QEvent::Close:
        instance->classId = smoke->findClass("QCloseEvent");
        break;
    case QEvent::Quit:
    case QEvent::ParentChange:
    case QEvent::ParentAboutToChange:
    case QEvent::ThreadChange:
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
    case QEvent::ShowToParent:
    case QEvent::HideToParent:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::Wheel:
        instance->classId = smoke->findClass("QWheelEvent");
        break;
    case QEvent::WindowTitleChange:
    case QEvent::WindowIconChange:
    case QEvent::ApplicationWindowIconChange:
    case QEvent::ApplicationFontChange:
    case QEvent::ApplicationLayoutDirectionChange:
    case QEvent::ApplicationPaletteChange:
    case QEvent::PaletteChange:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::Clipboard:
        instance->classId = smoke->findClass("QClipboardEvent");
        break;
    case QEvent::Speech:
    case QEvent::MetaCall:
    case QEvent::SockAct:
    case QEvent::WinEventAct:
    case QEvent::DeferredDelete:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::DragEnter:
        instance->classId = smoke->findClass("QDragEnterEvent");
        break;
    case QEvent::DragLeave:
        instance->classId = smoke->findClass("QDragLeaveEvent");
        break;
    case QEvent::DragMove:
        instance->classId = smoke->findClass("QDragMoveEvent");
    case QEvent::Drop:
        instance->classId = smoke->findClass("QDropEvent");
        break;
    case QEvent::DragResponse:
        instance->classId = smoke->findClass("QDragResponseEvent");
        break;
    case QEvent::ChildAdded:
    case QEvent::ChildRemoved:
    case QEvent::ChildPolished:
        instance->classId = smoke->findClass("QChildEvent");
        break;
    case QEvent::ShowWindowRequest:
    case QEvent::PolishRequest:
    case QEvent::Polish:
    case QEvent::LayoutRequest:
    case QEvent::UpdateRequest:
    case QEvent::EmbeddingControl:
    case QEvent::ActivateControl:
    case QEvent::DeactivateControl:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::ContextMenu:
        instance->classId = smoke->findClass("QContextMenuEvent");
        break;
    case QEvent::DynamicPropertyChange:
        instance->classId = smoke->findClass("QDynamicPropertyChangeEvent");
        break;
    case QEvent::InputMethod:
        instance->classId = smoke->findClass("QInputMethodEvent");
        break;
    case QEvent::AccessibilityPrepare:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::TabletMove:
    case QEvent::TabletPress:
    case QEvent::TabletRelease:
        instance->classId = smoke->findClass("QTabletEvent");
        break;
    case QEvent::LocaleChange:
    case QEvent::LanguageChange:
    case QEvent::LayoutDirectionChange:
    case QEvent::Style:
    case QEvent::OkRequest:
    case QEvent::HelpRequest:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::IconDrag:
        instance->classId = smoke->findClass("QIconDragEvent");
        break;
    case QEvent::FontChange:
    case QEvent::EnabledChange:
    case QEvent::ActivationChange:
    case QEvent::StyleChange:
    case QEvent::IconTextChange:
    case QEvent::ModifiedChange:
    case QEvent::MouseTrackingChange:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::WindowBlocked:
    case QEvent::WindowUnblocked:
    case QEvent::WindowStateChange:
        instance->classId = smoke->findClass("QWindowStateChangeEvent");
        break;
    case QEvent::ToolTip:
    case QEvent::WhatsThis:
        instance->classId = smoke->findClass("QHelpEvent");
        break;
    case QEvent::StatusTip:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::ActionChanged:
    case QEvent::ActionAdded:
    case QEvent::ActionRemoved:
        instance->classId = smoke->findClass("QActionEvent");
        break;
    case QEvent::FileOpen:
        instance->classId = smoke->findClass("QFileOpenEvent");
        break;
    case QEvent::Shortcut:
        instance->classId = smoke->findClass("QShortcutEvent");
        break;
    case QEvent::WhatsThisClicked:
        instance->classId = smoke->findClass("QWhatsThisClickedEvent");
        break;
    case QEvent::ToolBarChange:
        instance->classId = smoke->findClass("QToolBarChangeEvent");
        break;
    case QEvent::ApplicationActivated:
    case QEvent::ApplicationDeactivated:
    case QEvent::QueryWhatsThis:
    case QEvent::EnterWhatsThisMode:
    case QEvent::LeaveWhatsThisMode:
    case QEvent::ZOrderChange:
        instance->classId = smoke->findClass("QEvent");
        break;
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        instance->classId = smoke->findClass("QHoverEvent");
        break;
    case QEvent::AccessibilityHelp:
    case QEvent::AccessibilityDescription:
        instance->classId = smoke->findClass("QEvent");
    case QEvent::GraphicsSceneMouseMove:
    case QEvent::GraphicsSceneMousePress:
    case QEvent::GraphicsSceneMouseRelease:
    case QEvent::GraphicsSceneMouseDoubleClick:
        instance->classId = smoke->findClass("QGraphicsSceneMouseEvent");
        break;
    case QEvent::GraphicsSceneContextMenu:
        instance->classId = smoke->findClass("QGraphicsSceneContextMenuEvent");
        break;
    case QEvent::GraphicsSceneHoverEnter:
    case QEvent::GraphicsSceneHoverMove:
    case QEvent::GraphicsSceneHoverLeave:
        instance->classId = smoke->findClass("QGraphicsSceneHoverEvent");
        break;
    case QEvent::GraphicsSceneHelp:
        instance->classId = smoke->findClass("QGraphicsSceneHelpEvent");
        break;
    case QEvent::GraphicsSceneDragEnter:
    case QEvent::GraphicsSceneDragMove:
    case QEvent::GraphicsSceneDragLeave:
    case QEvent::GraphicsSceneDrop:
        instance->classId = smoke->findClass("QGraphicsSceneDragDropEvent");
        break;
    case QEvent::GraphicsSceneWheel:
        instance->classId = smoke->findClass("QGraphicsSceneWheelEvent");
        break;
    case QEvent::KeyboardLayoutChange:
        instance->classId = smoke->findClass("QEvent");
        break;
    default:
        break;
    }
    return;
}

static void
qobjectTypeResolver(JSmoke::Object::Instance * instance)
{
    Smoke * smoke = instance->classId.smoke;    
    QObject * qobject = static_cast<QObject*>( smoke->cast( instance->value, 
                                                            instance->classId,
                                                            JSmoke::Global::QObjectClassId ) );
    const QMetaObject * meta = qobject->metaObject();

    while (meta != 0) {
        Smoke::ModuleIndex classId = smoke->findClass(meta->className());
        if (classId != smoke->NullModuleIndex) {
            instance->classId = classId;
            return;
        }

        meta = meta->superClass();
    }
    
    return;
}

static QScriptValue 
QtEnum_toString(QScriptContext* context, QScriptEngine* /*engine*/)
{
    return context->thisObject().property("value").toString();
}

static QScriptValue 
QtEnum_valueOf(QScriptContext* context, QScriptEngine* /*engine*/)
{
    return context->thisObject().property("value");
}

/*
    The Qt.Enum class is used for marshalling enum values. It has a 'typeName' property
    with the type name of the enum, and a 'value' property. The 'typeName' is used
    when overloaded methods need to be resolved on enum types of the arguments.
 */
static QScriptValue 
QtEnum_ctor(QScriptContext* context, QScriptEngine* engine)
{
     QScriptValue object;
     if (context->isCalledAsConstructor()) {
         object = context->thisObject();
     } else {
         object = engine->newObject();
         object.setPrototype(context->callee().property("prototype"));
     }
     
     object.setProperty("value", context->argument(0));
     object.setProperty("typeName", context->argument(1));
     object.setProperty("valueOf", engine->newFunction(QtEnum_valueOf));
     object.setProperty("toString", engine->newFunction(QtEnum_toString));
     return object;
}

static QScriptValue 
Debug_trace(QScriptContext* context, QScriptEngine* engine)
{
    JSmoke::Debug::DoDebug = context->argument(0).toUInt32();
    return engine->undefinedValue();
}

static QScriptValue 
Debug_ctor(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue object = engine->newObject();
    object.setPrototype(context->callee().property("prototype"));
     
    object.setProperty("None", 0);
    object.setProperty("MethodMatches", 1);
    object.setProperty("Properties", 2);
    object.setProperty("Calls", 4);
    object.setProperty("GC", 8);
    object.setProperty("Virtual", 16);
    object.setProperty("Verbose", 32);
    
    object.setProperty("trace", engine->newFunction(Debug_trace), QScriptValue::PropertySetter);
    return object;
}

static QScriptValue 
QVariant_valueOf(QScriptContext* context, QScriptEngine* engine)
{
    JSmoke::Object::Instance * instance = JSmoke::Object::Instance::get(context->thisObject());
    if (instance == 0) {
        return context->thisObject();
    }
    
    QVariant * variant = static_cast<QVariant*>(instance->value);   
    return JSmoke::valueFromVariant(engine, *variant);
}

static QScriptValue 
QVariant_fromValue(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue value = context->argument(0);
    QVariant variant = JSmoke::valueToVariant(value);
    QVariant * copy = new QVariant(variant);
    QScriptValue result = JSmoke::Global::wrapInstance(engine, qtcore_Smoke->findClass("QVariant"), copy);
    return result;
}

namespace JSmoke {
extern Marshall::TypeHandler QtCoreHandlers[];
extern void registerQtCoreTypes(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_qt_core_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_qtcore_Smoke();
        JSmoke::Module qtcore_module = { "qtcore", new JSmoke::Binding(qtcore_Smoke) };
        JSmoke::Global::modules[qtcore_Smoke] = qtcore_module;    
    
        JSmoke::Global::QObjectClassId = qtcore_Smoke->idClass("QObject");
        JSmoke::Global::QDateClassId = qtcore_Smoke->idClass("QDate");
        JSmoke::Global::QDateTimeClassId = qtcore_Smoke->idClass("QDateTime");
        JSmoke::Global::QTimeClassId = qtcore_Smoke->idClass("QTime");
        JSmoke::Global::QEventClassId = qtcore_Smoke->idClass("QEvent");

        JSmoke::Marshall::installHandlers(JSmoke::QtCoreHandlers);
        
        JSmoke::Global::registerTypeResolver(JSmoke::Global::QEventClassId, qeventTypeResolver);
        JSmoke::Global::registerTypeResolver(JSmoke::Global::QObjectClassId, qobjectTypeResolver );
                                                        
        JSmoke::Global::startFinalizerThread();
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    JSmoke::Global::initializeClasses(engine, qtcore_Smoke);
               
    QScriptValue QtClass = engine->globalObject().property(QString("Qt"));
    JSmoke::Global::QtEnum = engine->newFunction(QtEnum_ctor);
    QtClass.setProperty("Enum", JSmoke::Global::QtEnum);            
    QtClass.setProperty("Debug", engine->newFunction(Debug_ctor).call());

    QScriptValue QVariantClass = engine->globalObject().property(QString("QVariant"));
    QVariantClass.setProperty("fromValue", engine->newFunction(QVariant_fromValue));
    QVariantClass.property("prototype").setProperty("valueOf", engine->newFunction(QVariant_valueOf));

    JSmoke::registerQtCoreTypes(engine);
        
    return;
}
