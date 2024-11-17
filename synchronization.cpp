#include "synchronization.h"

#include <QDir>

#include "folder.h"
#include "file.h"
#include "syncfile.h"



Synchronization::Synchronization(QObject *parent, QString sync_folder, QTcpSocket* socket) : QObject(parent), sync_folder(sync_folder), socket(socket)
{
    qDebug() << "Synchronization(sync_folder)" << sync_folder;
    //sync_folder.append("%2F");
    qDebug() << "sync_folder" << sync_folder;

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
        qDebug() << "foreach" << file_name;
        //if(file_name == "SyncFile.json") continue;
        File file(this, dir.filePath(file_name), syncfile);
        // QString pathaddfilename = path;
        // pathaddfilename.append("%2F");
        // pathaddfilename.append(file_name);
        // File file(this, pathaddfilename, syncfile);
    }

    QStringList folder_names = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QString folder_name, folder_names) {
        QString sub_folder_path = dir.filePath(folder_name);
        this->sync(sub_folder_path);
    }

    folder->closeFolder();
    delete folder;

    qDebug() << "Folder " << path << " and all contents sent successfully!";

}

void Synchronization::send(const QByteArray& arr)
{
    qDebug() << "send() << size" << arr.size();

    socket->write(arr);
    socket->waitForBytesWritten();
}

bool Synchronization::allBytesSend()
{
    qDebug() << "allBytesSend()";

    if(socket->bytesToWrite() == 0) return true;
    else return false;
}

QString Synchronization::getSyncFolder()
{
    qDebug() << "getSyncFolder()";

    return sync_folder;
}



