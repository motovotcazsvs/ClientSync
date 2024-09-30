#include "authorization.h"

#include <QDataStream>

Authorization::Authorization(QObject *parent) : QObject(parent)
{
    QObject::connect(socket, &QTcpSocket::readyRead, this, &Authorization::authorizationClient);

}

void Authorization::authorizationClient()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_7);
    quint16 size;
    QString type;

    if(socket->bytesAvailable() < 2) return;//якщо менше поля розмір то вийти
    in >> size;
    if(socket->bytesAvailable() < size) return;

    in >> type;
    qDebug() << socket->socketDescriptor() << "Type Authorization: " << type;
    if(type == "ID"){
        in >> id;
        if(id == 0){
            id = settingsfile->newID();
            sendID();
            return;
        }
        folder_id = settingsfile->getFolderID(id);
        client->setID(id);
        client->setFolderID(folder_id);
        client->authorizationSuccessful();

    }
}

