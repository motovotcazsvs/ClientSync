#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket;
    settingsfile = new SettingsFile("settingsfile.json");
    authorization = new Authorization(this, this, settingsfile);
}

void Client::authorizationSuccessful()
{
    for(int i = 0; i < settingsfile->getCountSync(); i++){
        Synchronization* newsynchronization = new Synchronization(this, settingsfile->getPathSync(i));
        synchronizations.append(newsynchronization);
    }
    delete authorization;
}

void Client::newSync(QString path)
{
    settingsfile->addSync(path);
    Synchronization* newsynchronization = new Synchronization(this, path);
    synchronizations.append(newsynchronization);
}

void Client::deleteSync(int num)
{
    synchronizations.removeAt(num);
}
