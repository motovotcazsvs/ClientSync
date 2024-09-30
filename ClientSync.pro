TEMPLATE = app

QT += qml quick network
CONFIG += c++11

SOURCES += main.cpp \
    client.cpp \
    authorization.cpp \
    synchronization.cpp \
    settingsfile.cpp \
    syncfile.cpp \
    file.cpp \
    folder.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    client.h \
    authorization.h \
    synchronization.h \
    settingsfile.h \
    syncfile.h \
    file.h \
    folder.h
