#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>



class Synchronization : public QObject
{
    Q_OBJECT
public:
    explicit Synchronization(QObject *parent = nullptr, QString sync_folder = "");
    void send(const QByteArray&);
    bool allBytesSend();

signals:

public slots:
    void startSync();

private:
    QString sync_folder;
    QTimer* sync_timer;
    QTcpSocket* socket;
    void sync(const QString&);

};

#endif // SYNCHRONIZATION_H
