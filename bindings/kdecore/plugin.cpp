#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_kdecore_bindings(QScriptValue &);

class jsmoke_kdecore_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_kdecore_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.kdecore");
    return list;
}

void jsmoke_kdecore_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.kdecore")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_kdecore_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_kdecore::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_kdecore_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_kdecore, jsmoke_kdecore_ScriptPlugin)
