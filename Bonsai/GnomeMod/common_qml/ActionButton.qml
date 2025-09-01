import QtQuick.Controls.Material
import QtQuick
import QtQuick.Controls

// Button{
//     id: rightButton
//     implicitWidth: 100
//     implicitHeight: 100

//     Button{
//         id: leftButton
//         implicitWidth: 120
//         implicitHeight: parent.height

//         Component.onCompleted: {
//             leftButton.background.color = "red"
//         }

//     }
// }


// Button{
//         Button{
//             anchors.fill: parent
//             background: Rectangle{
//                 anchors.fill: parent
//             }
//         }
// }

Button{
    id: root

    property bool active: true

    property alias leftText: leftButton.text
    property alias rightTex: rightButton.text

    property alias leftIconSource: leftButton.icon.source
    property color iconColorActive : "pink"
    property color iconColorInactive: "orange"

    property color colorLeftNormal: "#e3e3e3"
    property color colorLeftHover: "#d9d9d9"
    property color colorLeftPressed: "#c2c2c2"

    property color colorLeftActiveNormal: "#0073e5"
    property color colorLeftActiveHover: "#0069d1"
    property color colorLeftActivePressed: "#0052a3"

    property color colorRightNormal: "#e6e6e6"
    property color colorRightHover: "#dddddd"
    property color colorRightPressed: "#c8c8c8"

    property color colorRightActiveNormal: "#1981e8"
    property color colorRightActiveHover: "#1978d6"
    property color colorRightActivePressed: "#1963ac"



    Button{
        id: leftButton

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        anchors.right: separator.left
        Material.foreground: root.active ? root.iconColorActive : root.iconColorInactive

        background: Rectangle{
             color:  root.active ? (leftButton.hovered ? (leftButton.pressed ? (root.colorLeftActivePressed) : (root.colorLeftActiveHover)) : (root.colorLeftActiveNormal)):(leftButton.hovered ? (leftButton.pressed ? (root.colorLeftPressed) : (root.colorLeftHover)) : (root.colorLeftNormal))
             anchors.fill: parent
             topLeftRadius: height/2
             bottomLeftRadius: height/2
        }
        onClicked: {
            root.active = !root.active
        }
        // Component.onCompleted: {
        //      leftButton.contentItem.color = "white";
        // }
    }
    Rectangle{
        id: separator
        width: 1
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        anchors.right: rightButton.left
        color: root.active ? "#195fa3" : "#cfcfcf"
    }

    Button{
        id: rightButton

        width: 35
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6

        icon.source: "file:" + HOME + "/Bonsai/themes/gnome/images/go-next-symbolic.svg"
        icon.width: 15
        icon.height: 15
        leftPadding: 0
        topPadding: 0
        bottomPadding: 0
        rightPadding: 0
        Material.foreground: root.active ? root.iconColorActive : root.iconColorInactive

        background: Rectangle{
            color:  root.active ? (rightButton.hovered ? (rightButton.pressed ? (root.colorRightActivePressed) : (root.colorRightActiveHover)) : (root.colorRightActiveNormal)):(rightButton.hovered ? (rightButton.pressed ? (root.colorRightPressed) : (root.colorRightHover)) : (root.colorRightNormal))
            anchors.fill: parent
            topRightRadius: height/2
            bottomRightRadius: height/2
        }
    }

}

// Row{
//     id: rightButton
//     // implicitWidth: 100
//     // implicitHeight: 100
//     Button{

//     }
//     Button{

//     }
// }
