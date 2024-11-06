#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

#include <QTimer>

#include "authorization.h"
#include "settingsfile.h"
#include "synchronization.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    Q_INVOKABLE void newSync(QString);
    Q_INVOKABLE void deleteSync(int);
    Q_INVOKABLE void onoffSynchronizations(bool);
    Q_INVOKABLE QStringList getSyncList();


signals:


public slots:
    void startSynchronizations();

private slots:
    void authorizationSuccessfull();

private:
    Authorization* authorization;
    SettingsFile* settingsfile;
    QVector<Synchronization*> synchronizations;
    QTcpSocket* socket;
    QTimer* sync_timer;
    bool on_off_sync;

};

#endif // CLIENT_H
