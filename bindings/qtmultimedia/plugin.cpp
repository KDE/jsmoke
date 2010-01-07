#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtmultimedia_bindings(QScriptValue &);

class jsmoke_qtmultimedia_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtmultimedia_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtmultimedia");
    return list;
}

void jsmoke_qtmultimedia_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtmultimedia")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtmultimedia_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtmultimedia::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtmultimedia_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtmultimedia, jsmoke_qtmultimedia_ScriptPlugin)
