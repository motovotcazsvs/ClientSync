#include "client.h"
#include <QUrl>

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    settingsfile = new SettingsFile("settingsfile.json");
    //socket->connectToHost("192.168.0.108", 44);
    socket->connectToHost("192.168.0.171", 44);
    authorization = new Authorization(this, settingsfile, socket);
    QObject::connect(authorization, &Authorization::successfull, this, &Client::authorizationSuccessfull);

}

void Client::authorizationSuccessfull()
{
    qDebug() << "authorizationSuccessfull()";

    for(int i = 0; i < settingsfile->getCountSync(); i++){
        Synchronization* newsynchronization = new Synchronization(this, settingsfile->getPathSync(i));
        synchronizations.append(newsynchronization);
    }
    delete authorization;
}

void Client::newSync(QString path)
{
    qDebug() << "newSync(path)" << path;

    QUrl url(path);
    QString localPath = url.toLocalFile();
    path = localPath;
    qDebug() << "path" << path;

    settingsfile->addSync(path);
    Synchronization* newsynchronization = new Synchronization(this, path, socket);
    synchronizations.append(newsynchronization);
}

void Client::deleteSync(int num)
{
    qDebug() << "deleteSync(num)" << num;

    QString path = synchronizations.at(num)->getSyncFolder();
    synchronizations.removeAt(num);
    settingsfile->removeSync(path);

}

void Client::onoffAllSync(bool on_off)
{
    qDebug() << "onoffAllSync(on_off)" << on_off;

    for(int i = 0; i < synchronizations.count(); i++){
        synchronizations.at(i)->onoffTimer(on_off);
    }
}

void Client::onoffSync(bool on_off, int num)
{
    qDebug() << "onoffSync(on_off, num)" << on_off << num;

    synchronizations.at(num)->onoffTimer(on_off);
}

QStringList Client::getSyncList()
{
    qDebug() << "getSyncList()";

    return settingsfile->getSync();
}
