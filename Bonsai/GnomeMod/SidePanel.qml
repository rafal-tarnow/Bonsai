import QtQuick
import QtQuick.Controls

Window{
    id: root
    color: "#72000000"

    signal launcherButtonClicked()

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.topMargin: 6
        anchors.bottom: allProgramsButton.top
        spacing: 3

        Repeater {
            id: favoritesRepeater
            model: favoriteAppsModel

            delegate: Button {
                id: button
                width: parent.width
                height: width
                hoverEnabled: true

                icon.source: "image://backend_app_icon/" + model.icon
                icon.width: 48
                icon.height: 48

                background: Rectangle{
                    anchors.fill: parent
                    color: button.hovered ? (button.down ? "#3cffffff" : "#1cffffff") : ("#00000000")
                    border.width: 0
                    radius: 12

                    Behavior on color {
                        ColorAnimation {
                            duration: 150
                        }
                    }
                }

                onClicked: {
                    backend.runCommand(model.exec)
                }

            }
        }
        Rectangle{
            height: 1
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 6
            color: "#464646"
        }

        Repeater {
            id: taskBarRepeater
            model: backend_taskbarModel

            delegate: Button {
                id: taskButton
                width: parent.width
                height: width
                hoverEnabled: true

                icon.source: model.windowIcon
                icon.width: 48
                icon.height: 48

                background: Rectangle{
                    anchors.fill: parent
                    color: taskButton.hovered ? (taskButton.down ? "#3cffffff" : "#1cffffff") : ("#00000000")
                    border.width: 0
                    radius: 12

                    Behavior on color {
                        ColorAnimation {
                            duration: 150
                        }
                    }
                }

                onClicked: {
                    backend.activateWindow(model.windowId)
                }

            }
        }
    }
    Button{
        id: allProgramsButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        height: allProgramsButton.width

        icon.source: "file:" + HOME + "/Bonsai/themes/gnome/images/launcher.svg"
        icon.height: allProgramsButton.height - 30
        icon.width: allProgramsButton.height - 30

        background: Rectangle{
            color: allProgramsButton.hovered ? (allProgramsButton.down ? "#3cffffff" : "#1cffffff") : ("#00000000")
            border.width: 0
            radius: 12

            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }
        }
        onClicked: {
            root.launcherButtonClicked();
        }
    }
}
