#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtgui_bindings(QScriptValue &);

class jsmoke_qtgui_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtgui_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtgui");
    return list;
}

void jsmoke_qtgui_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtgui")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtgui_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtgui::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtgui_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_qtgui, jsmoke_qtgui_ScriptPlugin)
