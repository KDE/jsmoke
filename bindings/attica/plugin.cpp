#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_attica_bindings(QScriptValue &);

class jsmoke_attica_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_attica_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.attica");
    return list;
}

void jsmoke_attica_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.attica")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_attica_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_attica::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_attica_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_attica, jsmoke_attica_ScriptPlugin)
