#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_jsmoke_qtxmlpatterns_bindings(QScriptValue &);

class jsmoke_qtxmlpatterns_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList jsmoke_qtxmlpatterns_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.qtxmlpatterns");
    return list;
}

void jsmoke_qtxmlpatterns_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.qtxmlpatterns")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_jsmoke_qtxmlpatterns_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "jsmoke_qtxmlpatterns::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(jsmoke_qtxmlpatterns_ScriptPlugin)
Q_EXPORT_PLUGIN2(jsmoke_qtxmlpatterns, jsmoke_qtxmlpatterns_ScriptPlugin)
