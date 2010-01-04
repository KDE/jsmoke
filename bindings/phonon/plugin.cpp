#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_phonon_bindings(QScriptValue &);

class jsmoke_phonon_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_phonon_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.phonon");
    return list;
}

void jsmoke_phonon_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.phonon")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_phonon_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_phonon::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_phonon_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_phonon, jsmoke_phonon_ScriptPlugin)
