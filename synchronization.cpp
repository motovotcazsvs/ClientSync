#include "synchronization.h"

#include <QDir>

#include "folder.h"
#include "file.h"
#include "syncfile.h"

#define SYNCTIMER 30000//600000


Synchronization::Synchronization(QObject *parent, QString sync_folder, QTcpSocket* socket) : QObject(parent), sync_folder(sync_folder), socket(socket)
{
    qDebug() << "Synchronization(sync_folder)" << sync_folder;

    sync_timer = new QTimer;

    QObject::connect(sync_timer, &QTimer::timeout, this, &Synchronization::startSync);
}

void Synchronization::startSync()
{
    qDebug() << "startSync()";

    this->sync(sync_folder);
}

void Synchronization::sync(const QString& path)
{
    qDebug() << "sync()" << path;

    if(!socket->isValid()){
        qDebug() << "Socket is not valid, skipping file transfer.";
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist!";
        return;
    }

    SyncFile syncfile(path);

    Folder* folder = new Folder(this, path);
    folder->openFolder();

    QStringList file_names = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    foreach(QString file_name, file_names){
        if(file_name == "SyncFile.json") continue;
        File file(this, dir.filePath(file_name), syncfile);
    }

    QStringList folder_names = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString folder_name, folder_names) {
        QString sub_folder_path = dir.filePath(folder_name);
        this->sync(sub_folder_path);
    }

    folder->closeFolder();
    delete folder;

    qDebug() << "Folder " << path << " and all contents sent successfully!";


    this->onoffTimer(false);//for debug
}

void Synchronization::send(const QByteArray& arr)
{
    qDebug() << "send()";

    socket->write(arr);
    socket->waitForBytesWritten();
}

bool Synchronization::allBytesSend()
{
    qDebug() << "allBytesSend()";

    if(socket->bytesToWrite() == 0) return true;
    else return false;
}

void Synchronization::onoffTimer(bool on_off)
{
    qDebug() << "onoffTimer(on_off)" << on_off;

    if(on_off) sync_timer->start(SYNCTIMER);
    else sync_timer->stop();
}

QString Synchronization::getSyncFolder()
{
    qDebug() << "getSyncFolder()";

    return sync_folder;
}



