#include "settingsfile.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>


SettingsFile::SettingsFile(QString path)
{
    file_path = path;
    id = 0;
    count_sync = 0;
    loadSettings();
}

quint64 SettingsFile::getID()
{
    return id;
}
int SettingsFile::getCountSync()
{
    return path_sync_list.count();
}

QString SettingsFile::getPathSync(int num)
{
    return path_sync_list.at(num);
}

void SettingsFile::addSync(QString sync)
{
    path_sync_list.append(sync);
    saveSettings();
}

void SettingsFile::setID(quint64 new_id)
{
    id = new_id;
    saveSettings();
}

bool SettingsFile::loadSettings()
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("error open file for read");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning("error format json");
        return false;
    }

    QJsonObject json = doc.object();

    // Зчитуємо ID
    if (json.contains("id") && json["id"].isDouble())
        id = json["id"].toVariant().toULongLong();

    // Зчитуємо список шляхів
    if (json.contains("sync_paths") && json["sync_paths"].isArray())
    {
        QJsonArray pathArray = json["sync_paths"].toArray();
        for (const auto& path : pathArray)
        {
            if (path.isString())
                path_sync_list.append(path.toString());
        }
    }

    return true;
}

bool SettingsFile::saveSettings()
{
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning("error open file for write");
        return false;
    }

    QJsonObject json;
    json["id"] = static_cast<double>(id);

    QJsonArray pathArray;
    for(const QString& path : path_sync_list)
    {
        pathArray.append(path);
    }
    json["sync_paths"] = pathArray;

    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();

    return true;
}



