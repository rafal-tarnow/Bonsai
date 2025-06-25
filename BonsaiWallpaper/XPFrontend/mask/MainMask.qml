import QtQuick
import QtQuick.Controls
import "../"
import Bonsai.Backend


// WindowOverlay{

// }


Window {
    id: root
    objectName: "wind1"

    title: qsTr("Wallpaper window")
    visibility: Window.FullScreen
    width: Screen.width
    height: Screen.height
    visible: true
    color: "pink"
    flags: Qt.WindowStaysOnBottomHint

    Component.onCompleted: {
        backend.setX11WindowTypeAsDesktop(root)
    }


    Timer {
        interval: 5000 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {
            backend.windowManager.unloadEffect("kwin4_effect_fadingpopups")
            backend.windowManager.unloadEffect("kwin4_effect_scale")

            backend.windowManager.unloadEffect("fadingpopups")
            backend.windowManager.unloadEffect("scale")
        }
    }

    WindowOverlay{

    }


    RemoteWindow{
        id: appMenu
        source: "qrc:///AppMenuWindow.qml"
        //source: "qrc:///Clock3D.qml"
        //source: "qrc:///MainTest.qml"
        visible: true
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.leftMargin: 0
        width: 380
        height: 478
        swapInterval: 1
    }

    WallpaperContent{
        anchors.fill: parent
        Button{
            anchors.top: parent.top
            anchors.right: parent.right
            text: "Menu"
            onClicked: {
                appMenu.visible = false
            }
        }
    }
}


