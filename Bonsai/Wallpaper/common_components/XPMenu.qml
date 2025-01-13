import QtQuick
import QtQuick.Controls

Menu {
    id: contextMenu
    implicitWidth: 127
    implicitHeight: 229
    topPadding: 3
    leftPadding: 3
    rightPadding: 3
    //popupType: Popup.Window
    popupType: Popup.Native
    //popupType: Popup.Item
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    background: Rectangle{
        color: "#ffffff"
        border.width: 1
        border.color: "#aca899"
    }
}





