#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_kutils_bindings(QScriptValue &);

class jsmoke_kutils_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_kutils_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.kutils");
    return list;
}

void jsmoke_kutils_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.kutils")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_kutils_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_kutils::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_kutils_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_kutils, jsmoke_kutils_ScriptPlugin)
