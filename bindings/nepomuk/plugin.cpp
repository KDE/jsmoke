#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_nepomuk_bindings(QScriptValue &);

class jsmoke_nepomuk_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_nepomuk_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.nepomuk");
    return list;
}

void jsmoke_nepomuk_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.nepomuk")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_nepomuk_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_nepomuk::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_nepomuk_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_nepomuk, jsmoke_nepomuk_ScriptPlugin)
