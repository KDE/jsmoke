#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_sopranoclient_bindings(QScriptValue &);

class jsmoke_sopranoclient_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_sopranoclient_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.sopranoclient");
    return list;
}

void jsmoke_sopranoclient_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.sopranoclient")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_sopranoclient_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_sopranoclient::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_sopranoclient_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_sopranoclient, jsmoke_sopranoclient_ScriptPlugin)
