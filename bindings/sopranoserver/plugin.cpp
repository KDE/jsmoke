#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_org_kde_sopranoserver_bindings(QScriptValue &);

class org_kde_sopranoserver_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList org_kde_sopranoserver_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.sopranoserver");
    return list;
}

void org_kde_sopranoserver_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.sopranoserver")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_org_kde_sopranoserver_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "org_kde_sopranoserver::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(org_kde_sopranoserver_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_org_kde_sopranoserver, org_kde_sopranoserver_ScriptPlugin)