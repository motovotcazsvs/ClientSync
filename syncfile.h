#ifndef SYNCFILE_H
#define SYNCFILE_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QMap>

class SyncFile
{
public:
    SyncFile(const QString&);
    bool needToCheck();
    bool fileChanged(const QString&);
    void saveChanged(const QString&);


private:
    QMap<QString, QDateTime> sync_file_list;
    bool already_synchronization;
    QString path;
    bool loadSyncFile(const QString&);
    bool saveSyncFile();

};

#endif // SYNCFILE_H
