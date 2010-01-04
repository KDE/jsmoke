#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_kfile_bindings(QScriptValue &);

class jsmoke_kfile_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_kfile_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.kfile");
    return list;
}

void jsmoke_kfile_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.kfile")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_kfile_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_kfile::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_kfile_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_kfile, jsmoke_kfile_ScriptPlugin)
