#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qimageblitz_bindings(QScriptValue &);

class jsmoke_qimageblitz_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qimageblitz_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qimageblitz");
    return list;
}

void jsmoke_qimageblitz_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qimageblitz")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qimageblitz_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qimageblitz::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qimageblitz_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_qimageblitz, jsmoke_qimageblitz_ScriptPlugin)
