import QtQuick
import QtQuick.Controls

Image {
    id: wallpaper
    anchors.fill: parent
    source: "qrc:/assets/images/images/xp_fhd.jpg"

    Button{
        anchors.top: parent.top
        anchors.right: parent.right
        text: "Gnome"
        onClicked: {
            backend.setActiveFrontend("Gnome")
        }
    }

    Clock3D{
        id: clock3D

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.right: parent.right
    }
}
