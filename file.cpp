#include "file.h"

#include <QFile>
#include <QDataStream>

#include "synchronization.h"
#include "syncfile.h"

File::File(Synchronization* synchronization, const QString& path, SyncFile& syncfile) : synchronization(synchronization)
{
    QFile file(path);

    if(syncfile.needToCheck()){
        if(syncfile.fileChanged(path)){
            this->metaData(file);
            this->fileData(file);
        }
        else return;
    }
    else {
        this->metaData(file);
        this->fileData(file);
    }

    syncfile.saveChanged(path);
}

void File::metaData(QFile& file)
{
    QByteArray arr;
    QString name_file = file.fileName();
    qDebug() << "name_file" << name_file;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << QString("FILE") << name_file << file.size();
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    synchronization->send(arr);
}

void File::fileData(QFile& file)
{
    QByteArray block;
    if (file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            block = file.read(512);
            synchronization->send(block);
        }
        file.close();
    }

    if(synchronization->allBytesSend()) qDebug() << "file upload completed!";

}




