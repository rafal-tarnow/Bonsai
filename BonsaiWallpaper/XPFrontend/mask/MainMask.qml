import QtQuick
import QtQuick.Controls
import "../"

Window {
    id: root
    objectName: "wind1"

    title: qsTr("Hello World")
    visibility: Window.FullScreen
    width: Screen.width
    height: Screen.height
    visible: true
    color: "pink"
    flags: Qt.WindowStaysOnBottomHint

    Component.onCompleted: {
        backend.setX11WindowTypeAsDesktop(root)
    }

    WindowOverlay{

    }

    WallpaperContent{
        anchors.fill: parent
    }
}
