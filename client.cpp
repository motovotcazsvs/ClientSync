#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    settingsfile = new SettingsFile(settingsfile.json);
    authorization = Authorization(this, this, settingsfile);
}

void Client::authorizationSuccessful()
{
    for(int i = 0; i < settingsfile->getCountSync(); i++){
        Synchronization* newsynchronization = new Synchronization(settingsfile->getPathSync(i));
        synchronizations.append(newsynchronization);
    }
    delete authorization;
}

void Client::newSync(QString path)
{
    settingsfile->addSync(path);
    Synchronization* newsynchronization = new Synchronization(path);
    synchronizations.append(newsynchronization);
}
