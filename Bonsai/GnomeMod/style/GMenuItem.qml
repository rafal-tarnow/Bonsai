import QtQuick
import QtQuick.Controls
import "../"

MenuItem {
    id: root
    leftPadding: 0
    rightPadding: 0

    property bool active: true

    contentItem: Text{

        leftPadding: 32
        rightPadding: 32
        text: root.text
        verticalAlignment: Text.AlignVCenter
        color: root.active ? UIStyle.colorControlBoardIcon : "#9c9c9c"
    }

    // arrow: Canvas {
    //     x: parent.width - width
    //     implicitWidth: implicitHeight
    //     implicitHeight: root.height
    //     visible: root.subMenu
    //     onPaint: {
    //         var ctx = getContext("2d")
    //         //ctx.fillStyle = menuItem.highlighted ? "#ffffff" : "#21be2b"
    //         ctx.fillStyle = "#21be2b"
    //         ctx.moveTo(0, 0)
    //         ctx.lineTo(width - 15, height / 2)
    //         ctx.lineTo(15, height - 15)
    //         ctx.closePath()
    //         ctx.fill()

    //         // ctx.fillStyle = "#21be2b"
    //         // ctx.moveTo(0, 0)
    //         // ctx.lineTo(10, 0)
    //         // ctx.lineTo(10, 10)
    //         // ctx.lineTo(0, 10)
    //         // ctx.closePath()
    //         // ctx.fill()
    //     }

    // }

    arrow: Item{
        x: parent.width - width
        implicitWidth: implicitHeight
        implicitHeight: root.height
        visible: root.subMenu

        Button {
            id: iconBtn
            height: parent.height * 0.8
            width: parent.width * 0.8
            anchors.centerIn: parent

            icon.source: "../icons/right.svg"
            //icon.source: "../icons/right-large.svg"
            icon.color: UIStyle.colorButtonSystemSettingsIcon
            icon.width: iconBtn.width
            icon.height: iconBtn.height

            display: AbstractButton.IconOnly

            background: null
        }
    }

    background: Rectangle{
        color: root.active ? (root.highlighted ? ("#dedede") : (root.hovered ? "#dedede" : "#FFFFFF")) : "#FFFFFF"
        radius: 4
        Behavior on color {

            ColorAnimation {
                duration: 60
            }
        }
    }
}
