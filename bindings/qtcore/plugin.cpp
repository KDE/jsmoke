#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtcore_bindings(QScriptValue &);

class jsmoke_qtcore_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtcore_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtcore");
    return list;
}

void jsmoke_qtcore_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtcore")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtcore_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtcore::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtcore_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtcore, jsmoke_qtcore_ScriptPlugin)
