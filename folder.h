#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>

class Synchronization;
class Folder
{
public:
    Folder(Synchronization* synchronization = nullptr, const QString& folder_path = "");
    void openFolder();
    void closeFolder();

private:
    Synchronization* synchronization;
    const QString path;
};

#endif // FOLDER_H
