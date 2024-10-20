#include "syncfile.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

SyncFile::SyncFile(const QString& path) : path(path)
{
    qDebug() << "SyncFile()" << path;

    QString file_path = path + "/SyncFile.json";
    if(QFile(file_path).exists() == false){
        QFile sync_file(file_path);
        already_synchronization = false;
        sync_file.open(QIODevice::WriteOnly);
        sync_file.close();
    }
    else{
        already_synchronization = true;
        this->loadSyncFile(file_path);
    }

}

bool SyncFile::needToCheck()
{
    qDebug() << "needToCheck()";

    return already_synchronization;
}

bool SyncFile::fileChanged(const QString& file_path)
{
    qDebug() << "fileChanged(file_path)" << file_path;

    QFileInfo file_info(file_path);
    QString file_name = file_info.fileName();
    QDateTime last_modified = file_info.lastModified();
    if(sync_file_list.contains(file_name)) {
        QDateTime last_sync_time = sync_file_list.value(file_name);
        if(last_modified > last_sync_time){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return true;
    }
}

void SyncFile::saveChanged(const QString& file_path)
{
    qDebug() << "saveChanged()" << file_path;

    QFileInfo file_info(file_path);
    QString file_name = file_info.fileName();
    QDateTime last_modified = file_info.lastModified();

    sync_file_list[file_name] = last_modified;
    saveSyncFile();
}

bool SyncFile::loadSyncFile(const QString& file_path)
{
    qDebug() << "loadSyncFile(file_path)" << file_path;

    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("no open file for write!");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning("json format error!");
        return false;
    }

    QJsonObject json = doc.object();
    QJsonArray sync_files = json["sync_files"].toArray();

    // Зчитуємо дані з JSON масиву
    for (const QJsonValue& value : sync_files) {
        QJsonObject file_obj = value.toObject();
        QString file_name = file_obj["file_name"].toString();
        QDateTime last_sync = QDateTime::fromString(file_obj["last_sync"].toString(), Qt::ISODate);

        sync_file_list[file_name] = last_sync;
    }

    return true;
}

bool SyncFile::saveSyncFile()
{
    qDebug() << "saveSyncFile()";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("no open file for write!");
        return false;
    }

    QJsonArray sync_files;
    for (auto it = sync_file_list.constBegin(); it != sync_file_list.constEnd(); ++it) {
        QJsonObject file_obj;
        file_obj["file_name"] = it.key();
        file_obj["last_sync"] = it.value().toString(Qt::ISODate);
        sync_files.append(file_obj);
    }

    QJsonObject json;
    json["sync_files"] = sync_files;

    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();

    return true;
}

