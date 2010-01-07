#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtxml_bindings(QScriptValue &);

class jsmoke_qtxml_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtxml_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtxml");
    return list;
}

void jsmoke_qtxml_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtxml")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtxml_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtxml::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtxml_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtxml, jsmoke_qtxml_ScriptPlugin)
