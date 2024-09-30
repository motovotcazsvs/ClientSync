#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <QObject>
#include <QTimer>

#include "syncfile.h"


class Synchronization : public QObject
{
    Q_OBJECT
public:
    explicit Synchronization(QObject *parent = nullptr, QString);

signals:

public slots:

private:
    QString sync_folder;
    QString current_path;
    SyncFile* syncfile;
    QTimer* sync_timer;

};

#endif // SYNCHRONIZATION_H
