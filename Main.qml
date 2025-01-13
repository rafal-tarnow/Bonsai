import QtQuick
import QtQuick.Controls

Window {    
    width: Screen.width
    height: Screen.height
    visibility: "FullScreen"  // Ustawiamy pełny ekran

    title: qsTr("Hello World")

    Rectangle {
           anchors.fill: parent
           color: "red"  // Ustawienie koloru tła na czerwony
    }

    Button {
        id: logoutButton
        text: "Logout"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20
        onClicked: {
            mainAppObject.logout()
        }
    }
    Button {
        id: rebootButton
        text: "Reboot"
        anchors.top: logoutButton.bottom
        anchors.left: logoutButton.left
        onClicked: {
            mainAppObject.reboot()
        }
    }
    Button {
        text: "Poweroff"
        anchors.top: rebootButton.bottom
        anchors.left: rebootButton.left
        onClicked: {
            mainAppObject.poweroff()
        }
    }

}
