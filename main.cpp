#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "client.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Client client;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("client", &client);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
