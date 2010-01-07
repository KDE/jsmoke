#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_kparts_bindings(QScriptValue &);

class jsmoke_kparts_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_kparts_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.kparts");
    return list;
}

void jsmoke_kparts_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.kparts")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_kparts_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_kparts::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_kparts_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_kparts, jsmoke_kparts_ScriptPlugin)
