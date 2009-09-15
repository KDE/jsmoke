#ifndef SAMPLECLASS_H
#define SAMPLECLASS_H

#include <QScriptClass>
#include <smoke.h>

//so that one instance of ImplementationClass can serve all objects
//maybe this isn't worth it though
struct AttributedObject
{
    QByteArray className;
    void* object;
};
Q_DECLARE_METATYPE( AttributedObject* )

class ImplementationClass;

//I'm open to a better name :)
//!The QScriptValue of QWidget, QListView etc. So it's used for construction and to access class methods.
class StaticClass : public QScriptClass
{
    public:
        StaticClass( QScriptEngine* );
        ~StaticClass();
        QScriptValue prototype() const;
        QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id );
        QScriptValue property ( const QScriptValue & object, const QScriptString & name, uint id );
        QVariant extension( QScriptClass::Extension extension, const QVariant& argument );
        bool supportsExtension( QScriptClass::Extension extension ) const;
    private:
        ImplementationClass* m_protoClass;
};

#include <QScriptContext>
Q_DECLARE_METATYPE( QScriptContext* )

#endif