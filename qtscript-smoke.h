#ifndef qtscript-smoke_H
#define qtscript-smoke_H

#include <QtCore/QObject>

class qtscript-smoke : public QObject
{
Q_OBJECT
public:
    qtscript-smoke();
    virtual ~qtscript-smoke();
private slots:
    void output();
};

#endif // qtscript-smoke_H
