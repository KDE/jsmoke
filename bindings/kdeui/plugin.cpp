#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_kdeui_bindings(QScriptValue &);

class jsmoke_kdeui_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_kdeui_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.kdeui");
    return list;
}

void jsmoke_kdeui_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.kdeui")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_kdeui_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_kdeui::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_kdeui_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_kdeui, jsmoke_kdeui_ScriptPlugin)
