#include "file.h"

#include <QFile>
#include <QDataStream>
#include <QUrl>
#include "synchronization.h"
#include "syncfile.h"



File::File(Synchronization* synchronization, const QString& path, SyncFile& syncfile) : synchronization(synchronization)
{
    qDebug() << "File(path)" << path;
    QString pathadd = path;
    //pathadd.prepend("%2F");
    qDebug() << "pathadd" << pathadd;
    QFile file(pathadd);
    qint64 size_file = file.size();
    qDebug() << "size_file" << size_file;

    QString name_file = extractFileName(pathadd);
    qDebug() << "name_file" << name_file;

    if(syncfile.needToCheck()){
        if(syncfile.fileChanged(pathadd)){
            this->metaData(name_file, size_file);
            this->fileData(file);
        }
        else return;
    }
    else {
        this->metaData(name_file, size_file);
        this->fileData(file);
    }

    syncfile.saveChanged(pathadd);
}

QString File::extractFileName(const QString& uri)
{
    // Спочатку декодуємо URI
    QString decodedUri = uri;
    decodedUri = QUrl::fromPercentEncoding(decodedUri.toUtf8());

    // Знаходимо останній слеш
    int lastSlashIndex = decodedUri.lastIndexOf('/');
    if (lastSlashIndex != -1) {
        // Повертаємо частину після останнього слешу, що є ім'ям файлу
        return decodedUri.mid(lastSlashIndex + 1);
    }
    return QString();
}

void File::metaData(QString& name_file, qint64 size_file)
{
    qDebug() << "metaData()";

    QByteArray arr;
    qDebug() << "name_file" << name_file;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << QString("FILE") << name_file << size_file;
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    synchronization->send(arr);
}

void File::fileData(QFile& file)
{
    qDebug() << "fileData()";

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




