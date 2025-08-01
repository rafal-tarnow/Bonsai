import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtWebEngine
import Bonsai.Backend

import "./common_components"
import "./windows"
import "./popups"
import "../"



Window {
    id: root
    title: qsTr("Bonsai Wallpaper")
    objectName: "wind1"  //allow to find window on C++ side: QQuickWindow *window = engine.rootObjects().first()->findChild<QQuickWindow*>("wind1");

    visible: true

    width: Screen.width
    height: Screen.height
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnBottomHint //| Qt.X11BypassWindowManagerHint
    visibility: Window.FullScreen

    Component.onCompleted: {
        backend.installAuroraeTheme("qrc:/assets/kwin_themes/XBoomer")
        backend.windowManager.setAuroraeTheme("XBoomer")
    }


    //----------------------------------------


    //readonly property Vol.SinkModel.

    //property var sink:

    //----------------------------------------



    Timer {
        interval:  128//128 * 3 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        //8ms - FAIL
        //16ms - FAIL
        //32ms - FAIL
        //64ms - FAIL
        //128ms - OK
        //256ms - OK
        //512ms - OK
        running: true
        repeat: false
        onTriggered: {
            //BWindowManagerX11.reconfigure()

            BWindowManagerX11.unloadEffect("kwin4_effect_fadingpopups")
            BWindowManagerX11.unloadEffect("kwin4_effect_scale")

            BWindowManagerX11.unloadEffect("fadingpopups")
            BWindowManagerX11.unloadEffect("scale")


            BWindowManagerX11.hideFromTaskbar(root, true);
            BWindowManagerX11.hideFromSwitcher(root, true);
            BWindowManagerX11.hideFromPager(root, true);

            backend.setX11WindowTypeAsDesktop(root)
        }
    }

    WallpaperContent {
        id: wallpaper
        anchors.fill: parent

        onLeftMouseButtonClicked:{
            if(systemVolumeSlider.visible)
                systemVolumeSlider.visible = false
            if(appMenu.visible)
                appMenu.visible = false
            console.log("Wallpapre Left clicked 2")
        }

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




    //AppMenuPopup{
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

    // Column{
    //     anchors.right: parent.right
    //     anchors.verticalCenter: parent.verticalCenter

    //     Button{
    //         text: "clock3d visible"
    //         onClicked: {
    //             clock3D.proxyVisible = !clock3D.proxyVisible
    //         }
    //     }
    // }

    // BProxyWindow{
    //     id: clock3D
    //     source: "qrc:///windows/Clock3DWindow.qml"
    //     //source: "qrc:///Clock3D.qml"
    //     //source: "qrc:///MainTest.qml"
    //     proxyVisible: true
    //     anchors.bottom: panel.top
    //     anchors.right: panel.right
    //     width: 200
    //     height: 200
    //     swapInterval: 1
    // }

    TaskbarContent{
        id: panel
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 100
        anchors.right: parent.right
        anchors.left: parent.left

        onStartPressed: {
            appMenu.visible = !appMenu.visible
        }

        onEndPressed: {

        }

        property size dimensions: Qt.size(width, height)
        onDimensionsChanged: {
            backend.reservePanelBottomArea(root, panel.x, panel.y, panel.width, panel.height)
        }
    }

    //SystemVolumePopup{
    SystemVolumeWindow{
        id: systemVolumeSlider
        x: panel.x + panel.width - width - 20
        y: panel.y-height

        visible: false

    }


}

