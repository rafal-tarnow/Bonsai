import QtQuick
import QtQuick.Controls
import "../"
import "../common_components"
import "../contents"

Popup {
    popupType:  Popup.Window

    width: 81
    height: 136

    contentItem: SystemVolumeContent{
        anchors.fill: parent
    }
}
