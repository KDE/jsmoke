#ifndef SAMPLECLASS_H
#define SAMPLECLASS_H

#include <QScriptClass>
#include <smoke.h>

struct AttributedObject
{
    QByteArray className;
    void* object;
};
Q_DECLARE_METATYPE( AttributedObject* )

class SampleImpl;

class SampleClass : public QScriptClass
{
    public:
        SampleClass( QScriptEngine* );
        ~SampleClass();
        QScriptValue prototype() const;
        QueryFlags queryProperty( const QScriptValue & object, const QScriptString & name, QueryFlags flags, uint * id );
        QScriptValue property ( const QScriptValue & object, const QScriptString & name, uint id );
        QVariant extension( QScriptClass::Extension extension, const QVariant& argument );
        bool supportsExtension( QScriptClass::Extension extension ) const;
    private:
        SampleImpl* m_protoClass;
};

#include <QScriptContext>
Q_DECLARE_METATYPE( QScriptContext* )

#endif