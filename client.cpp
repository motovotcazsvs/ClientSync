#include "client.h"
#include <QUrl>
#include <QDir>
#include <QStandardPaths>


#define SYNCTIMER 30000//600000

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    settingsfile = new SettingsFile("settingsfile.json");
    qDebug() << "Файл буде створено в: " << QDir::currentPath() + "/settingsfile.json";

    //QString filePath = "/storage/emulated/0/settingsfile.json";
    //QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settingsfile.json";
    //settingsfile = new SettingsFile(filePath);
    //qDebug() << "Файл буде створено в: " << filePath;

    //socket->connectToHost("192.168.0.108", 44);
    socket->connectToHost("192.168.0.151", 44);
    //socket->connectToHost("192.168.0.171", 44);
    //socket->connectToHost("2a02:2378:11c1:2a1c:854:8272:925d:a3b8", 44);
    authorization = new Authorization(this, settingsfile, socket);
    QObject::connect(authorization, &Authorization::successfull, this, &Client::authorizationSuccessfull);
    this->sync_timer = new QTimer;
    QObject::connect(sync_timer, &QTimer::timeout, this, &Client::startSynchronizations);
}

void Client::authorizationSuccessfull()
{
    qDebug() << "authorizationSuccessfull()";

    for(int i = 0; i < settingsfile->getCountSync(); i++){
        Synchronization* newsynchronization = new Synchronization(this, settingsfile->getPathSync(i), socket);
        synchronizations.append(newsynchronization);
    }
    delete authorization;
}

void Client::newSync(QString path)
{
    qDebug() << "newSync(path)" << path;

    // QUrl url(path);
    // QString localPath = url.toLocalFile();
    // path = localPath;
    // qDebug() << "path" << path;



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

void Client::onoffSynchronizations(bool on_off)
{
    qDebug() << "onoffSynchronizations(on_off)" << on_off;

    if(on_off){
        this->startSynchronizations();
    }
    else {
        this->sync_timer->stop();
    }

}

QStringList Client::getSyncList()
{
    qDebug() << "getSyncList()";

    return settingsfile->getSync();
}

void Client::startSynchronizations()
{
    qDebug() << "startSynchronizations()";

    this->sync_timer->stop();
    emit this->stateSync(true);
    for(int i = 0; i < synchronizations.count(); i++){
        qDebug() << "for i" << i << synchronizations.count();
        synchronizations.at(i)->startSync();
    }
    this->sync_timer->start(SYNCTIMER);
    emit this->stateSync(false);
}

