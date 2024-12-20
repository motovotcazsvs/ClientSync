#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFile>

class Synchronization;
class SyncFile;
class File
{
public:
    File(Synchronization* Synchronization, const QString&, SyncFile&);
    void metaData(QString&, qint64);
    void fileData(QFile&);

private:
    Synchronization* synchronization;

};

#endif // FILE_H
