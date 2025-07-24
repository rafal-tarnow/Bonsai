import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "./common_components"
import "./contents"
import Bonsai.Backend

Image {
    id: wallpaper
    anchors.fill: parent
    source: "qrc:/assets/images/images/xp_fhd.jpg"


    // Button{
    //     anchors.centerIn: parent
    //     text: "Logout"
    //     onClicked: {
    //         console.log("logout clicked")
    //         BSessionManager.logout()
    //     }
    // }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: (mouse)=> {
            if (mouse.button === Qt.RightButton) {
                recentAppsContextMenu.popup(mouse.x, mouse.y)
            }
        }
    }

    BFrontendManager{
        id: frontendManager
    }

    ListView {
        id: frontendListView
        anchors.centerIn: parent
        width: 300
        height: 300
        model: frontendManager
        clip: true

        delegate: ItemDelegate {
            width: parent.width
            height: 120

            Rectangle {
                anchors.fill: parent
                color: frontendActive ? "lightgreen" : "white"
                border.color: "#ddd"
                radius: 5

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5

                    Label {
                        text: "Name: " + frontendName
                        font.bold: true
                    }
                    Label {
                        text: "ID: " + frontendId
                    }
                    Label {
                        text: "Description: " + frontendDescription
                    }
                    Label {
                        text: "Path: " + frontendPath
                        font.italic: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        frontendManager.switchFrontend(frontendId)
                    }
                }
            }
        }

        ScrollBar.vertical: ScrollBar {}
    }

    XPMenu {
        id: recentAppsContextMenu
        z: 1
        x: 100
        y: 100
        width: 127
        height: 135 + 17
        visible: true
        popupType: Popup.Window

        XPMenuItem {
            text: "Arrange Icons By"
            active: false
            onTriggered: {
            }
        }

        XPMenuItem {
            text: "Refresh"
            active: false
        }

        XPMenu {
            id: frontendSubmenu
            title: "Change Frontend"
            height: 40
            // XPMenuItem {
            //     text: "XP Luna"
            //     active: false
            // }
            // XPMenuItem {
            //     text: "Gnome"
            //     active: true
            //     onTriggered: {
            //         backend.setActiveFrontend("Gnome")
            //     }
            // }

            enabled: frontendsInstantiator.count > 0

            Instantiator {
                id: frontendsInstantiator
                model: frontendManager
                delegate: XPMenuItem {
                    text: model.frontendName
                    active: !model.frontendActive
                    onTriggered: {
                        console.log("1-- onTrigger() new frontend = " + model.frontendId)
                        frontendManager.activeFrontend = model.frontendId
                    }
                }

                onObjectAdded: (index, object) => frontendSubmenu.insertItem(index, object)
                onObjectRemoved: (index, object) => frontendSubmenu.removeItem(object)
            }
        }

        XPMenuSeparator{
        }

        XPMenuItem {
            text: "Paste"
            active: false
        }

        XPMenuItem {
            text: "Paste Shortcut"
            active: false
        }

        XPMenuSeparator{
        }

        XPMenuItem {
            text: "New"
            active: false
        }

        // XPMenu {
        //     title: "New"
        //     //active: false
        //     //     Action { text: "Find Next" }
        //     //     Action { text: "Find Previous" }
        //     //     Action { text: "Replace" }
        // }

        XPMenuSeparator{
        }

        XPMenuItem {
            text: "Properties"
            active: false
        }
    }


    Rectangle{
        id: cpuLoadBackground
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15
        width: 150
        height: 37
        color: "#dd000000"
        Text{
            id: cpuLoad
            anchors.centerIn: parent
            text: "CPU: " + backend.cpuLoad.toFixed(1) + "%"
            font.pixelSize: 24
            color: "white"

            Component.onCompleted: {
                backend.measureCpuLoad = true
            }
        }
    }

    Rectangle{
        id: qtVersionBackground
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: cpuLoadBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#dd000000"
        Text{
            anchors.centerIn: parent
            text: "Qt: " + backend.qtVersion
            font.pixelSize: 24
            color: "white"
        }

    }

    Rectangle{
        id: dpiBackground
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: qtVersionBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#dd000000"
        Text{
            anchors.centerIn: parent
            text: "DPI: " + (Screen.pixelDensity * 25.4).toFixed(2)
            font.pixelSize: 24
            color: "white"
        }

    }
}


