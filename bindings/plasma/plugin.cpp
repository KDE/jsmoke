#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_plasma_bindings(QScriptValue &);

class jsmoke_plasma_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_plasma_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.plasma");
    return list;
}

void jsmoke_plasma_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.plasma")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_plasma_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_plasma::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_plasma_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_plasma, jsmoke_plasma_ScriptPlugin)
