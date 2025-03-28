import QtQuick
import QtQuick.Controls
import Bonsai.Backend
import QtQuick3D
import "../"

Window {
    id: root
    objectName: "wind1"

    title: qsTr("Hello World")
    visibility: Window.FullScreen
    width: Screen.width
    height: Screen.height
    visible: true
    //color: "#00000000"
    //color: "red"
    //color: "#88FFFFFF"
    color: "transparent"
    flags: Qt.WindowStaysOnTopHint
    //color: "#88ffffff"

    // onActiveChanged: {
    //      if(root.active === false)
    //          appMenuContent.visible = false
    // }

    onActiveFocusItemChanged: {
        if(root.activeFocusItem === null)
            appMenuContent.visible = false
    }

    Component.onCompleted: {
        //backend.setX11WindowTypeAsTopMenu(root)
        backend.setX11WindowTypeAsNotification(root)
    }

    Rectangle{
        id: backgroundRect
        anchors.fill: parent
        color: "#99FFFF00"
        visible: visibleBackgroundCheckbox.checked
    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: (mouse) => {
                       if (mouse.button === Qt.RightButton) {
                           wallpaperContextMenu.popup(mouse.x, mouse.y)
                       }
                   }
    }

    Menu {
        id: wallpaperContextMenu
        implicitHeight: 129
        popupType: Popup.Item
        closePolicy: Popup.CloseOnPressOutside

        MenuItem { text: "Cut" }
        MenuItem { text: "Copy" }
        MenuItem { text: "Paste" }
    }


    // Rectangle {
    //     width: 320
    //     height: 480
    //     color: "steelblue"

    //     ListView {
    //         anchors.fill: parent
    //         //! [model]
    //         model: Qt.fontFamilies()
    //         //! [model]

    //         delegate: Item {
    //             height: 40
    //             width: ListView.view.width
    //             required property string modelData
    //             Text {
    //                 anchors.centerIn: parent
    //                 text: parent.modelData
    //                 //! [delegate]
    //                 font.family: parent.modelData
    //                 //! [delegate]
    //                 font.pixelSize: 20
    //                 color: "white"
    //             }
    //         }
    //     }
    // }

    CheckBox{
        id: dbgMenu
        anchors.left: parent.left
        anchors.top: parent.top
        //text: "Debug menu"

        Component.onCompleted: {
            backend.addMaskedItem(dbgMenu)
        }
    }

    MaskedItem{
        id: dbgPanelMask
        width: 150
        height:300
        anchors.right: parent.right
        anchors.top: parent.top
        visible: dbgMenu.checked
        enabled: dbgMenu.checked

        Rectangle{
            anchors.fill: parent
            color: "green"
        }

        Column{
            id: buttonsRow

            Button{
                text: "Logout"
                onPressed: {
                    backend.logout()
                }
            }

            CheckBox{
                id: maskBackgroundCheckbox
                text: "Mask Background"

            }
            CheckBox{
                id: visibleBackgroundCheckbox
                text: "Visible Backgroun"
            }


            MenuSeparator{
            }

            CheckBox{
                id: orangeShowCheckbox
                text: "Orange show"
            }

            MenuSeparator {
            }

            CheckBox{
                id: blueShowCheckbox
                text: "Blue show"
            }

            CheckBox{
                id: blueAnimationCheckbox
                text: "Blue animation"
            }


            Button{
                text: "Qt.quit()"
                onClicked: {
                    Qt.quit()
                }
            }
        }
    }


    MaskedItem{
        x: draggableRect.x
        y: draggableRect.y
        width: draggableRect.width
        height: draggableRect.height
        visible: orangeShowCheckbox.checked
        enabled: orangeShowCheckbox.checked
    }


    Rectangle {
        id: draggableRect
        width: 20
        height: 20
        color: "orange"
        x: 450  // Początkowa pozycja X
        y: 50  // Początkowa pozycja Y

        visible: orangeShowCheckbox.checked
        enabled: orangeShowCheckbox.checked

        MouseArea {
            anchors.fill: parent  // Pokrywa cały Rectangle
            drag.target: draggableRect  // Obiekt do przeciągania
            drag.axis: Drag.XAndYAxis  // Umożliwia ruch w obu osiach
            drag.minimumX: 0  // Minimalna pozycja X (nie wychodzi poza lewą krawędź okna)
            drag.maximumX: parent.parent.width - width  // Maksymalna pozycja X (nie wychodzi poza prawą krawędź)
            drag.minimumY: 0  // Minimalna pozycja Y (nie wychodzi poza górną krawędź)
            drag.maximumY: parent.parent.height - height  // Maksymalna pozycja Y (nie wychodzi poza dolną krawędź)

            cursorShape: Qt.OpenHandCursor  // Zmiana kursora na "łapkę"
            onPressed: {
                cursorShape = Qt.ClosedHandCursor  // Zmiana kursora na "chwyt" podczas przeciągania
            }
            onReleased: {
                cursorShape = Qt.OpenHandCursor  // Powrót do "łapki" po puszczeniu
            }
        }
    }


    Rectangle{
        color: "blue"
        anchors.bottom: appMenuContent.bottom
        anchors.left: appMenuContent.left
        width: appMenuContent.width/2
        height: appMenuContent.height/2
        visible: false
    }


    // MaskedItem{
    //     x: appMenuContent.x
    //     y: appMenuContent.y
    //     width: appMenuContent.width
    //     height: appMenuContent.height
    //     visible: appMenuContent.visible
    // }

    LogoutContent{
        id: logoutContent
        anchors.fill: parent
        visible: false
        z: 2

        onCancelClicked: {
            visible = false
        }

        MaskedItem{
            anchors.fill: parent
            visible: parent.visible
        }
    }

    TurnOffContent{
        id: turnoffContent
        anchors.fill: parent
        visible: false
        z: 2

        onCancelClicked: {
            visible = false
        }

        MaskedItem{
            anchors.fill: parent
            visible: parent.visible
        }
    }

    AppMenuContent{
        id: appMenuContent
        visible: false
        anchors.bottom: panelMask.top
        anchors.left: panelMask.left
        z: 2

        onLogoutClicked: {
            logoutContent.visible = true
        }

        onTurnoffClicked: {
            turnoffContent.visible = true
        }

        Component.onCompleted: {
            backend.addMaskedItem(appMenuContent)
        }

        onHideRequest:{
            appMenuContent.visible = false
            appMenuContent.resetState()
        }


    }


    MaskedItem{
        id: panelMask
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: panel.height

        TaskbarContent{
            id: panel
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            inputMenuVisible: appMenuContent.visible

            onStartPressed: {
                appMenuContent.visible = !appMenuContent.visible
            }


            property size dimensions: Qt.size(width, height)
            onDimensionsChanged: {
                backend.reservePanelBottomArea(root, panel.x, panel.y, panel.width, panel.height)
            }

        }

    }








    MaskedItem {
        id: square
        width: 50
        height: 50

        x: points[0].x
        y: points[0].y
        visible: blueShowCheckbox.checked
        enabled: blueShowCheckbox.checked

        Rectangle{
            anchors.fill: parent
            color: "blue"
            border.width: 1
            border.color: "red"
        }


        property var points: [
            { x: 20, y: 20 },
            { x: 200, y: 100 },
            { x: 300, y: 300 },
            { x: 100, y: 800 },
            { x: 20, y: 20 }
        ]

        SequentialAnimation {
            id: pathAnimation
            running: blueAnimationCheckbox.checked
            loops: Animation.Infinite

            PropertyAnimation {
                target: square
                property: "x"
                to: square.points[1].x
                duration: 1000
            }
            PropertyAnimation {
                target: square
                property: "y"
                to: square.points[1].y
                duration: 1000
            }

            PropertyAnimation {
                target: square
                property: "x"
                to: square.points[2].x
                duration: 1000
            }
            PropertyAnimation {
                target: square
                property: "y"
                to: square.points[2].y
                duration: 1000
            }

            PropertyAnimation {
                target: square
                property: "x"
                to: square.points[3].x
                duration: 1000
            }
            PropertyAnimation {
                target: square
                property: "y"
                to: square.points[3].y
                duration: 1000
            }

            PropertyAnimation {
                target: square
                property: "x"
                to: square.points[4].x
                duration: 1000
            }
            PropertyAnimation {
                target: square
                property: "y"
                to: square.points[4].y
                duration: 1000
            }
        }
    }


}
