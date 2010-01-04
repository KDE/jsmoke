#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtopengl_bindings(QScriptValue &);

class jsmoke_qtopengl_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtopengl_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtopengl");
    return list;
}

void jsmoke_qtopengl_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtopengl")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtopengl_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtopengl::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtopengl_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_jsmoke_qtopengl, jsmoke_qtopengl_ScriptPlugin)
