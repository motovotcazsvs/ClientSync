#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "authorization.h"
#include "settingsfile.h"
#include "synchronization.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void authorizationSuccessful();

signals:

public slots:

private:
    Authorization* authorization;
    SettingsFile* settingsfile;
    QVector<Synchronization*> synchronizations;
    Q_INVOKABLE void newSync(QString);


};

#endif // CLIENT_H
