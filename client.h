#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

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
    Q_INVOKABLE void deleteSync(int);
    Q_INVOKABLE void onoffAllSync(bool);
    Q_INVOKABLE void onoffSync(bool, int);
    Q_INVOKABLE QStringList getSyncList();
    QTcpSocket* socket;


};

#endif // CLIENT_H
