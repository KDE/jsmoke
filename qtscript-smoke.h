#ifndef qtscript_smoke_H
#define qtscript_smoke_H

#include <QtCore/QObject>

class QtScriptSmoke : public QObject
{
    Q_OBJECT
    public:
        QtScriptSmoke();
        virtual ~QtScriptSmoke();
    private slots:
        void output();
};

#endif // qtscript-smoke_H
