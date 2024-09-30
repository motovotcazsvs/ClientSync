#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    settingsfile = new SettingsFile(settingsfile.json);
}
