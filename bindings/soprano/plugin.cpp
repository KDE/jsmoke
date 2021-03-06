#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_soprano_bindings(QScriptValue &);

class jsmoke_soprano_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_soprano_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.soprano");
    return list;
}

void jsmoke_soprano_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.soprano")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_soprano_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_soprano::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_soprano_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_soprano, jsmoke_soprano_ScriptPlugin)
