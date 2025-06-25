import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtWebEngine
import Bonsai.Backend

import "./common_components"



Window {
    id: root
    objectName: "wind1"  //allow to find window on C++ side: QQuickWindow *window = engine.rootObjects().first()->findChild<QQuickWindow*>("wind1");

    visible: true

    width: Screen.width
    height: Screen.height
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnBottomHint //| Qt.X11BypassWindowManagerHint
    visibility: Window.FullScreen
    title: qsTr("Bonsai 1")

    Component.onCompleted: {
        backend.installAuroraeTheme("qrc:/assets/kwin_themes/XBoomer")
        backend.windowManager.setAuroraeTheme("XBoomer")
        backend.windowManager.reconfigure()
    }

    RemoteWindow{
        source: "qrc:///AppMenuWindow.qml"
        //source: "qrc:///Clock3D.qml"
        //source: "qrc:///MainTest.qml"
        visible: true
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 0
        width: 380
        height: 478
    }

    //----------------------------------------


    //readonly property Vol.SinkModel.

    //property var sink:

    //----------------------------------------



    Timer {
        id: initTimer
        interval: 3000 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {

        }
    }

    Timer {
        interval: 5000 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {
            backend.setX11WindowTypeAsDesktop(root)
            backend.windowManager.unloadEffect("kwin4_effect_fadingpopups")
            backend.windowManager.unloadEffect("kwin4_effect_scale")

            backend.windowManager.unloadEffect("fadingpopups")
            backend.windowManager.unloadEffect("scale")
        }
    }

    WallpaperContent {
        id: wallpaper
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: (mouse) => {
                           if (mouse.button === Qt.LeftButton) {
                               if (appMenu.visible) {
                                   appMenu.visible = false;
                               }
                               if (systemVolumeSlidrer.visible) {
                                   systemVolumeSlidrer.visible = false;
                               }
                           }
                           if (mouse.button === Qt.RightButton) {
                               wallpaperContextMenu.popup(mouse.x, mouse.y)
                           }
                       }
        }
    }

    XPMenu {
        id: wallpaperContextMenu
        implicitHeight: 129
        popupType: Popup.Native

        XPMenuItem { text: "Cut" }
        XPMenuItem { text: "Copy" }
        XPMenuItem { text: "Paste" }
    }

    // WebEngineView {
    //     id: youtubeView
    //     width: 450
    //     height: 600
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     anchors.margins: 20
    //     url: "https://www.youtube.com/watch?v=izaO5-_nyp0&list=RDGMEMQ1dJ7wXfLlqCjwV0xfSNbA&index=2"  // Link osadzonego filmu z YouTube
    // }


    // View3D {
    //     id: view
    //     width: 600
    //     height: 600
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     focusPolicy: Qt.NoFocus


    //     PerspectiveCamera {
    //         position: Qt.vector3d(0, 200, 300)
    //         eulerRotation.x: -30
    //     }

    //     DirectionalLight {
    //         eulerRotation.x: -30
    //     }

    // Model {
    //     id: cube
    //     source: "#Cube"
    //     // materials: DefaultMaterial {
    //     //     diffuseColor: Qt.rgba(1, 0, 0, 1) // Ustaw kolor na czerwony
    //     // }
    //     materials: DefaultMaterial {
    //         diffuseMap: Texture {
    //             //sourceItem: dateTimeDisplay
    //             //sourceItem: wallpaper
    //             source: "qt_logo.png"
    //         }
    //     }
    //     eulerRotation.y: 90

    //     Vector3dAnimation on eulerRotation {
    //         loops: Animation.Infinite
    //         duration: 5000
    //         from: Qt.vector3d(0, 0, 0)
    //         to: Qt.vector3d(360, 0, 360)
    //     }
    // }


    Window{
        id: turnOffDialog

        x: root.x + (root.width - width) / 2
        y: root.y + (root.height - height) / 2

        color: "transparent"
        visibility: Window.Hidden

        TurnOffContent{
            anchors.fill: parent
            visible: turnOffDialog.visible
            onCancelClicked: {
                turnOffDialog.visibility = Window.Hidden
            }
        }
    }

    Window{
        id: logOffDialog

        x: root.x + (root.width - width) / 2
        y: root.y + (root.height - height) / 2

        color: "transparent"
        visibility: Window.Hidden

        LogoutContent{
            anchors.fill: parent
            visible: logOffDialog.visible
            onCancelClicked: {
                logOffDialog.visibility = Window.Hidden
            }
        }
    }






    Rectangle{
        id: cpuLoadBackground
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15
        width: 150
        height: 37
        color: "#aa000000"
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
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: cpuLoadBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#aa000000"
        Text{
            anchors.centerIn: parent
            text: "Qt: " + backend.qtVersion
            font.pixelSize: 24
            color: "white"
        }

    }

    Rectangle{
        id: dpiBackground
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: qtVersionBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#aa000000"
        Text{
            anchors.centerIn: parent
            text: "DPI: " + (Screen.pixelDensity * 25.4).toFixed(2)
            font.pixelSize: 24
            color: "white"
        }

    }



    // Column{
    //     anchors.top: parent.top
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     visible: true
    //     Row{
    //         TextField{
    //             placeholderText: appsListEffect.shadowBlur
    //         }
    //         Button{
    //             text: "shadowBlur : real"
    //             onClicked: {
    //                 //appsListEffect.shadowBlur = text
    //             }
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowHorizontalOffset : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowOpacity : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowScale : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowVerticalOffset : real"
    //         }
    //     }
    // }



    AppMenuWindow{
        id: appMenu
        x: panel.x
        y: panel.y-height

        width: 380
        height: 478

        visible: false

        onLogoutClicked: {
            logOffDialog.visibility = Window.FullScreen
        }
        onTurnoffClicked: {
            turnOffDialog.visibility = Window.FullScreen
        }
    }


    TaskbarContent{
        id: panel
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 100
        anchors.right: parent.right
        anchors.left: parent.left

        onStartPressed: {
            appMenu.visible = !appMenu.visible
        }

        property size dimensions: Qt.size(width, height)
        onDimensionsChanged: {
            backend.reservePanelBottomArea(root, panel.x, panel.y, panel.width, panel.height)
        }
    }

    SystemVolumeSlider{
        id: systemVolumeSlidrer
        x: panel.x + panel.width - width - 20
        y: panel.y-height

        visible: false

    }


    // ListView {
    //     id: themesList
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     width: 200
    //     height: 200
    //     clip: true
    //     model: backend.themesModel

    //     delegate: Rectangle {
    //         required property string themeId
    //         required property string themeName
    //         required property bool themeActive
    //         height: 50
    //         width: themesList.width
    //         border.width: 1

    //         Row{
    //             anchors.fill: parent
    //             RadioButton{
    //                 checked: themeActive
    //                 onClicked: {
    //                     //backend.themesModel.setActiveTheme(themeId)
    //                     backend.setActiveTheme(themeId)
    //                 }
    //             }
    //             Text{
    //                 text: themeName
    //             }
    //         }
    //     }
    // }
}

