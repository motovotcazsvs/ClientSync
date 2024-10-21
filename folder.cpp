#include "folder.h"

#include<QDataStream>
#include <QDebug>
#include <QDir>

#include "synchronization.h"

Folder::Folder(Synchronization* synchronization, const QString& folder_path) : synchronization(synchronization), path(folder_path)
{
    qDebug() << "Folder(folder_path)" << path;

    QDir dir(path);
    folder_name = dir.dirName();
}

void Folder::openFolder()
{
    qDebug() << "openFolder()";

    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << QString("START_FOLDER") << folder_name;
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    synchronization->send(arr);
    qDebug() << "send start folder";
}

void Folder::closeFolder()
{
    qDebug() << "closeFolder()";

    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << QString("END_FOLDER") << folder_name;
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    synchronization->send(arr);
    qDebug() << "send end folder";

}
