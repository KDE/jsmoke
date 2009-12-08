#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_org_kde_solid_bindings(QScriptValue &);

class org_kde_solid_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList org_kde_solid_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.solid");
    return list;
}

void org_kde_solid_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.solid")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_org_kde_solid_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "org_kde_solid::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(org_kde_solid_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_org_kde_solid, org_kde_solid_ScriptPlugin)
