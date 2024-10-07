#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H

#include <QObject>

class SettingsFile
{
public:
    SettingsFile(QString);
    quint64 getID();
    int getCountSync();
    QString getPathSync(int);
    void addSync(QString);
    void removeSync(QString);
    void setID(quint64);
    QStringList getSync();

private:
    quint64 id;
    QStringList path_sync_list;
    int count_sync;
    bool loadSettings();
    bool saveSettings();
    QString file_path;
};

#endif // SETTINGSFILE_H
