import QtQuick
import QtQuick.Controls
import "../"
import "../common_components"
import "../contents"

Window {
    flags:  Qt.Popup

    width: 81
    height: 136

    SystemVolumeContent{
        x: 0
        y: 0
        width: parent.width
        height: parent.height
    }
}
