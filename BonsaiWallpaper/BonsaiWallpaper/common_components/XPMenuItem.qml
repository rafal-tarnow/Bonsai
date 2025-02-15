import QtQuick
import QtQuick.Controls

MenuItem {
    id: root

    implicitHeight: 17
    implicitWidth: 121

    property alias fontStyleName: contentText.font.styleName
    property bool active: true

    contentItem: Text {
        id: contentText
        leftPadding: 10
        rightPadding: root.arrow.width
        text: root.text

        font.family: "Tahoma"
        font.styleName: "Regular"
        font.pointSize: 8.0
        font.kerning: true
        font.hintingPreference: Font.PreferFullHinting
        renderType: Text.NativeRendering
        antialiasing: false

        color: root.active ? (root.highlighted ? "#ffffff" : "#000000") : "#aca899"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        anchors.fill: parent
        implicitWidth: 200
        implicitHeight: 40
        color: root.highlighted ? "#316ac5" : "#ffffff"
    }
}
