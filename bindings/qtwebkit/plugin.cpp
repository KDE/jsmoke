#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtwebkit_bindings(QScriptValue &);

class jsmoke_qtwebkit_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtwebkit_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtwebkit");
    return list;
}

void jsmoke_qtwebkit_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtwebkit")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtwebkit_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtwebkit::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtwebkit_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtwebkit, jsmoke_qtwebkit_ScriptPlugin)
