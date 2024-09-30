#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QTcpSocket>

class Authorization : public QObject
{
    Q_OBJECT
public:
    explicit Authorization(QObject *parent = nullptr);

signals:

public slots:

private slots:
    void authorizationClient();

private:
    QTcpSocket* socket;
    quint64 id;
    SettingsFile* settingsfile;
};

#endif // AUTHORIZATION_H
