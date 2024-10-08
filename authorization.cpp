#include "authorization.h"

#include <QDataStream>

#include "client.h"
#include "settingsfile.h"

Authorization::Authorization(QObject* parent, Client* client, SettingsFile* settingsfile) : QObject(parent), client(client), settingsfile(settingsfile)
{
    id = settingsfile->getID();
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
        settingsfile->setID(id);
        sendID();
        client->authorizationSuccessful();

    }
}

void Authorization::sendID()
{
    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << quint16(0) << QString("ID") << id;//резервуєм два байта на розмір блоку(записуючи туди нулі) та поміщаєм дані в масив
    out.device()->seek(0);//переміщаємо вказівник на начало в масиві, тобто на зарезервовані два байта - розмір блоку
    out << quint16(arr.size() - sizeof(quint16));//та записуєм туди фактичний розмір даних(віднявши від масива перші два байти)
    qDebug() << arr;
    socket->write(arr);
    socket->waitForBytesWritten();

}


