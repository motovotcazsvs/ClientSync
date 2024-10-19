#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QTcpSocket>


class SettingsFile;
class Authorization : public QObject
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = nullptr, SettingsFile* settingsfile = nullptr, QTcpSocket* socket = nullptr);

signals:
    void successfull();

public slots:

private slots:
    void authorizationClient();

private:
    SettingsFile* settingsfile;
    QTcpSocket* socket;    
    quint64 id;
    void sendID();

};

#endif // AUTHORIZATION_H
