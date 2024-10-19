#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QTcpSocket>

class Client;
class SettingsFile;
class Authorization : public QObject
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = nullptr, Client* client = nullptr, SettingsFile* settingsfile = nullptr, QTcpSocket* socket = nullptr);

signals:
    void successfull();

public slots:

private slots:
    void authorizationClient();

private:
    QTcpSocket* socket;
    Client* client;
    SettingsFile* settingsfile;
    quint64 id;
    void sendID();

};

#endif // AUTHORIZATION_H
