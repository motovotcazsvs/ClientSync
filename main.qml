import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("ClientSync")

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

    Column {
        spacing: 20
        anchors.centerIn: parent

        Text {
            text: qsTr("ClientSync - Синхронізація Папок")
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Загальна кнопка синхронізації
        Button {
            id: syncButton
            text: "Увімкнути Синхронізацію"
            background: Rectangle {
                color: syncButton.text === "Увімкнути Синхронізацію" ? "red" : "green"
                radius: 4
            }
            onClicked: {
                if (syncButton.text === "Увімкнути Синхронізацію") {
                    syncButton.text = "Вимкнути Синхронізацію";
                    client.onoffAllSync(true);
                } else {
                    syncButton.text = "Увімкнути Синхронізацію";
                    client.onoffAllSync(false);
                }
            }
        }

        Button {
            text: "Додати Папку Синхронізації"
            onClicked: {
                fileDialog.open();
            }
        }

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
                        id: syncToggleButton
                        text: syncState === "ON" ? "Вимкнути" : "Увімкнути"
                        background: Rectangle {
                            color: syncState === "ON" ? "green" : "red"
                            radius: 4
                        }

                        onClicked: {
                            if (syncState === "OFF") {
                                syncState = "ON";
                                syncToggleButton.text = "Вимкнути";
                                console.log("Синхронізація увімкнена для: " + folderName);
                                client.onoffSync(true, index);
                            } else {
                                syncState = "OFF";
                                syncToggleButton.text = "Увімкнути";
                                console.log("Синхронізація вимкнена для: " + folderName);
                                client.onoffSync(false, index);
                            }
                        }
                    }

                    Button {
                        text: "Видалити"
                        onClicked: {
                            console.log("Видалено папку: " + folderName);
                            client.deleteSync(index)
                        }
                    }
                }
            }
        }
    }

}
