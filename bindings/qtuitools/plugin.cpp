#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtuitools_bindings(QScriptValue &);

class jsmoke_qtuitools_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtuitools_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtuitools");
    return list;
}

void jsmoke_qtuitools_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtuitools")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtuitools_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtuitools::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtuitools_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtuitools, jsmoke_qtuitools_ScriptPlugin)
