#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtsql_bindings(QScriptValue &);

class jsmoke_qtsql_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtsql_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtsql");
    return list;
}

void jsmoke_qtsql_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtsql")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtsql_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtsql::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtsql_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtsql, jsmoke_qtsql_ScriptPlugin)
