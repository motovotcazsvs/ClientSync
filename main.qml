import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ClientSync")



    Connections {
        target: client
        onStateSync: {
            if(state_sync) {
                main_text.text = qsTr("ЗАЧИКАЙТЕ іде Синхронізація Папок!");
                main_text.color = "red";
                syncButton.visible = false;
                bottom_add.visible = false;
            }
            else {
                main_text.text = qsTr("ClientSync - Синхронізація Папок");
                main_text.color = "black";
                syncButton.visible = true;
                bottom_add.visible = true;
            }
            console.log("синхронізація: " + state_sync);
        }
    }



    // Оголошення ListModel для зберігання папок
    ListModel {
        id: folderModel
    }


    Component.onCompleted: {
        var folders = client.getSyncList();
        for (var i = 0; i < folders.length; i++) {
            folderModel.append({"folderName": folders[i], "syncState": "OFF"});
        }
    }

    FileDialog {
        id: fileDialog
        folder: "file:///C:/"
        title: "Оберіть папку для синхронізації"
        selectFolder: true
        onAccepted: {
            console.log("Обрана папка: " + fileDialog.fileUrl);
            folderModel.append({"folderName": fileDialog.fileUrl.toString(), "syncState": "OFF"});
            client.newSync(fileDialog.fileUrl.toString());
        }
        onRejected: {
            console.log("Вибір папки скасовано");
        }
    }

    // Таймер для відкладеного запуску синхронізації
    Timer {
        id: syncTimer
        interval: 100 // затримка у мілісекундах
        repeat: false
        onTriggered: {
            client.onoffSynchronizations(true);  // Запуск синхронізації з C++
        }
    }

    Text {
        id: main_text
        text: qsTr("ClientSync - Синхронізація Папок")
        color: "black"
        font.pixelSize: 24
        anchors.top: parent.top
        anchors.topMargin: 25
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    // Загальна кнопка синхронізації
    Button {
        id: syncButton
        visible: true
        text: "Увімкнути Синхронізацію"
        anchors.top: main_text.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle {
            color: syncButton.text === "Увімкнути Синхронізацію" ? "red" : "green"
            radius: 4
        }
        onClicked: {
            if (syncButton.text === "Увімкнути Синхронізацію") {
                syncButton.text = "Вимкнути Синхронізацію";
                syncTimer.start();  // Запускаємо таймер
            } else {
                syncButton.text = "Увімкнути Синхронізацію";
                client.onoffSynchronizations(false);  // Вимкнення синхронізації
            }
        }
    }

    Button {
        id: bottom_add
        visible: true
        anchors.top: syncButton.bottom
        anchors.topMargin: 50
        text: "Додати Папку Синхронізації"
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            fileDialog.open();
        }
    }


    Column {
        spacing: 20
        anchors.top: bottom_add.bottom
        anchors.topMargin: 50
        anchors.left: parent.left
        ListView {
            width: parent.width
            height: 200
            model: folderModel

            delegate: Item {
                width: parent.width
                height: 40

                Row {
                    spacing: 10

                    Text {
                        text: folderName
                        font.pixelSize: 18
                    }

                    Button {
                        text: "Видалити"
                        onClicked: {
                            console.log("Видалено папку: " + folderName);
                            client.deleteSync(index);
                            folderModel.remove(index);
                        }
                    }
                }
            }
        }
    }
}



