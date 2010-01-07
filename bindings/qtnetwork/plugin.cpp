#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtnetwork_bindings(QScriptValue &);

class jsmoke_qtnetwork_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtnetwork_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtnetwork");
    return list;
}

void jsmoke_qtnetwork_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtnetwork")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtnetwork_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtnetwork::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtnetwork_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtnetwork, jsmoke_qtnetwork_ScriptPlugin)
