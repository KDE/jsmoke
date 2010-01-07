#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtsvg_bindings(QScriptValue &);

class jsmoke_qtsvg_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtsvg_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtsvg");
    return list;
}

void jsmoke_qtsvg_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtsvg")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtsvg_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtsvg::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtsvg_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtsvg, jsmoke_qtsvg_ScriptPlugin)
