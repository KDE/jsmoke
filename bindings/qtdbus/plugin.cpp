#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtdbus_bindings(QScriptValue &);

class jsmoke_qtdbus_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtdbus_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtdbus");
    return list;
}

void jsmoke_qtdbus_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtdbus")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtdbus_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtdbus::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtdbus_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtdbus, jsmoke_qtdbus_ScriptPlugin)
