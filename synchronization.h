#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QObject>
#include <QTcpSocket>



class Synchronization : public QObject
{
    Q_OBJECT
public:
    explicit Synchronization(QObject *parent = nullptr, QString sync_folder = "", QTcpSocket* socket = nullptr);
    void send(const QByteArray&);
    bool allBytesSend();
    QString getSyncFolder();
    void startSync();

signals:

public slots:


private:
    QString sync_folder;
    QTcpSocket* socket;
    void sync(const QString&);

};

#endif // SYNCHRONIZATION_H
