#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_knewstuff3_bindings(QScriptValue &);

class jsmoke_knewstuff3_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_knewstuff3_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.knewstuff3");
    return list;
}

void jsmoke_knewstuff3_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.knewstuff3")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_knewstuff3_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_knewstuff3::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_knewstuff3_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_knewstuff3, jsmoke_knewstuff3_ScriptPlugin)
