#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_knewstuff2_bindings(QScriptValue &);

class jsmoke_knewstuff2_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_knewstuff2_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.knewstuff2");
    return list;
}

void jsmoke_knewstuff2_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.knewstuff2")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_knewstuff2_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_knewstuff2::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_knewstuff2_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_knewstuff2, jsmoke_knewstuff2_ScriptPlugin)
