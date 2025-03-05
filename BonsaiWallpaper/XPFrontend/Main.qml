// import QtQuick

// Window {
//     width: 640
//     height: 480
//     visible: true
//     title: qsTr("Hello World")
//     Text{
//         anchors.centerIn: parent
//         text: "Module Wallpaper second XXXL"
//     }
// }



import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtWebEngine
//import BonsaiBackend


Window {
    id: root

    visible: true

    width: Screen.width
    height: Screen.height
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnBottomHint //| Qt.X11BypassWindowManagerHint
    visibility: Window.FullScreen
    title: qsTr("Bonsai 1")

    Component.onCompleted: {
        //backend.windowManager.installAuroraeTheme("qrc:/XBoomer")
        backend.windowManager.setAuroraeTheme("XBoomer")
        backend.windowManager.reconfigure()
    }

    //----------------------------------------


    //readonly property Vol.SinkModel.

    //property var sink:

    //----------------------------------------

    FontLoader {
        id: tahomaRegular
        source: "file:/" + HOME + "/Bonsai/themes/windows_xp/fonts/tahoma.ttf"
    }

    FontLoader {
        id: tahomaBold
        source: "file:/" + HOME + "/Bonsai/themes/windows_xp/fonts/tahomabd.ttf"
    }

    Timer {
        id: initTimer
        interval: 3000 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {

        }
    }

    Timer {
        interval: 5000 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {
            backend.setX11WindowTypeAsDesktop(root)
            backend.windowManager.unloadEffect("kwin4_effect_fadingpopups")
            backend.windowManager.unloadEffect("kwin4_effect_scale")

        }
    }

    Image {
        id: wallpaper
        anchors.fill: parent
        //x:0
        //y:0
        //width: parent.width
        //height: parent.height*0.75
        source: "file:/" + HOME + "/Bonsai/themes/windows_xp/images/xp_fhd.jpg"

        MouseArea {
            anchors.fill: parent
            // onClicked: {

            // }
            onPressed: {
                if(appMenu.visible){
                    appMenu.visible = false
                }
                if(systemVolumeSlidrer.visible){
                    systemVolumeSlidrer.visible = false
                }
            }
        }

    }












    // WebEngineView {
    //     id: youtubeView
    //     width: 450
    //     height: 600
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     anchors.margins: 20
    //     url: "https://www.youtube.com/watch?v=izaO5-_nyp0&list=RDGMEMQ1dJ7wXfLlqCjwV0xfSNbA&index=2"  // Link osadzonego filmu z YouTube
    // }


    // View3D {
    //     id: view
    //     width: 600
    //     height: 600
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     focusPolicy: Qt.NoFocus


    //     PerspectiveCamera {
    //         position: Qt.vector3d(0, 200, 300)
    //         eulerRotation.x: -30
    //     }

    //     DirectionalLight {
    //         eulerRotation.x: -30
    //     }

    // Model {
    //     id: cube
    //     source: "#Cube"
    //     // materials: DefaultMaterial {
    //     //     diffuseColor: Qt.rgba(1, 0, 0, 1) // Ustaw kolor na czerwony
    //     // }
    //     materials: DefaultMaterial {
    //         diffuseMap: Texture {
    //             //sourceItem: dateTimeDisplay
    //             //sourceItem: wallpaper
    //             source: "qt_logo.png"
    //         }
    //     }
    //     eulerRotation.y: 90

    //     Vector3dAnimation on eulerRotation {
    //         loops: Animation.Infinite
    //         duration: 5000
    //         from: Qt.vector3d(0, 0, 0)
    //         to: Qt.vector3d(360, 0, 360)
    //     }
    // }


    TurnOffWindow{
        id: turnOffDialog

        x: root.x + (root.width - width) / 2
        y: root.y + (root.height - height) / 2

        visibility: Window.Hidden
    }

    LogOffWindow{
        id: logOffDialog

        x: root.x + (root.width - width) / 2
        y: root.y + (root.height - height) / 2

        visibility: Window.Hidden
    }








    // Switch {
    //     id: cpuSwitch
    //     anchors.left: dateTimeDisplay.left
    //     anchors.bottom: dateTimeDisplay.top
    //     checked: backend.measureCpuLoad  // Powiąż stan przełącznika z właściwością measureCpuLoad

    //     // Obsługa zmiany stanu przełącznika
    //     onToggled: {
    //         backend.measureCpuLoad = cpuSwitch.checked
    //     }
    // }



    // Text {
    //     id: platformName
    //     anchors.left: dateTimeDisplay.left
    //     anchors.top: dateTimeDisplay.bottom
    //     text: "Platform: " + backend.platformName
    //     font.pixelSize: 24
    //     color: "white"
    // }


    Rectangle{
        id: cpuLoadBackground
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 15
        width: 150
        height: 37
        color: "#aa000000"
        Text{
            id: cpuLoad
            anchors.centerIn: parent
            text: "CPU: " + backend.cpuLoad.toFixed(1) + "%"
            font.pixelSize: 24
            color: "white"

            Component.onCompleted: {
                backend.measureCpuLoad = true
            }
        }
    }

    Rectangle{
        id: qtVersionBackground
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: cpuLoadBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#aa000000"
        Text{
            anchors.centerIn: parent
            text: "Qt: " + backend.qtVersion
            font.pixelSize: 24
            color: "white"
        }

    }

    Rectangle{
        id: dpiBackground
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: qtVersionBackground.bottom
        anchors.topMargin: 5
        width: 150
        height: 37
        color: "#aa000000"
        Text{
            anchors.centerIn: parent
            text: "DPI: " + (Screen.pixelDensity * 25.4).toFixed(2)
            font.pixelSize: 24
            color: "white"
        }

    }



    // Column{
    //     anchors.top: parent.top
    //     anchors.horizontalCenter: parent.horizontalCenter
    //     visible: true
    //     Row{
    //         TextField{
    //             placeholderText: appsListEffect.shadowBlur
    //         }
    //         Button{
    //             text: "shadowBlur : real"
    //             onClicked: {
    //                 //appsListEffect.shadowBlur = text
    //             }
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowHorizontalOffset : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowOpacity : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowScale : real"
    //         }
    //     }
    //     Row{
    //         TextField{

    //         }
    //         Button{
    //             text: "shadowVerticalOffset : real"
    //         }
    //     }
    // }



    AppMenuWindow{
        id: appMenu
        x: panel.x
        y: panel.y-height

        width: 380
        height: 478

        visible: false
    }


    Image {
        id: panel
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 100
        anchors.right: parent.right
        anchors.left: parent.left
        height: 30
        source: "file:/" + HOME + "/Bonsai/themes/windows_xp/task_bar_background.bmp"

        // Component.onCompleted: {

        // }

        property size dimensions: Qt.size(width, height)
        onDimensionsChanged: {
            backend.reservePanelBottomArea(root, panel.x, panel.y, panel.width, panel.height)
        }



        Image {
            id: startImage
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            height: parent.height
            width: 101
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_normal.bmp"
            fillMode: Image.TileHorizontally // lub inna opcja w zależności od wymagań

            Component.onCompleted: {
                console.log("WARNING TODO: It seems that a more optimal solution, instead of replacing the source for the Image, would be to create three Image elements and switch between them")
            }

            MouseArea {
                id: startButtonMouseArea
                anchors.fill: parent
                hoverEnabled: true

                Connections {
                    target: appMenu
                    function onVisibleChanged() {
                        if(appMenu.visible === false ){
                            if(!startButtonMouseArea.containsMouse){
                                startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_normal.bmp";
                            }
                        }
                    }
                }

                onEntered: {
                    if(appMenu.visible){
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_pressed.bmp";
                    }else{
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_hover.bmp";
                    }
                }
                onExited: {
                    if(appMenu.visible){
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_pressed.bmp";
                    }else{
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_normal.bmp";
                    }
                }
                onPressed: {
                    appMenu.visible = !appMenu.visible


                    if(appMenu.visible){
                        //backend.minimalizeAllWindows()
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_pressed.bmp";
                    }else{
                        if (containsMouse) {
                            startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_hover.bmp";
                        } else {
                            startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_normal.bmp";
                        }
                    }
                }
                onReleased: {
                    if(appMenu.visible){
                        startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_pressed.bmp";
                    }else{
                        if (containsMouse) {
                            startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_hover.bmp";
                        } else {
                            startImage.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/images/start_normal.bmp";
                        }
                    }
                }
            }
        }

        Row {
            id: taskbarRow
            readonly property int elementMinimumWidth: 52
            readonly property int elementMaximumWidth: 160
            property int availableWidth: width
            property int elementWidth: Math.min(availableWidth / taskbarRepeater.count, elementMaximumWidth)

            anchors.top: parent.top
            anchors.topMargin: 3
            anchors.bottom: parent.bottom
            anchors.left: startImage.right
            anchors.leftMargin: 5
            anchors.right: separator.left
            spacing: 0
            visible: true

            Repeater {
                id: taskbarRepeater
                model: backend_taskbarModel

                TaskbarButton{
                    id: taskButtonNormal

                    minimumWidth: taskbarRow.elementMinimumWidth
                    minimumHeight: 25

                    text: model.windowName
                    source: model.windowIcon

                    width: taskbarRow.elementWidth
                    height: 25

                    property bool activeWindow : model.windowActive

                    //works good with onRelease
                    buttonState: activeWindow ? (buttonMouseArea.containsMouse ? (buttonMouseArea.pressed ? TaskbarButton.ButtonState.Pressed :  TaskbarButton.ButtonState.PressedHover) /* containsMouse = false */: (TaskbarButton.ButtonState.Pressed)) /*activeWindow = false*/: (buttonMouseArea.containsMouse ? (buttonMouseArea.pressed?  TaskbarButton.ButtonState.Pressed : TaskbarButton.ButtonState.NormalHover) /* containsMouse = false */ : (TaskbarButton.ButtonState.Normal))

                    MouseArea {
                        id: buttonMouseArea
                        anchors.fill: parent
                        hoverEnabled: true

                        // TODO: Here, there is a difference in behavior compared to Windows XP.
                        //       After pressing the button, the focus and active window are taken
                        //       over by the panel, which doesn’t happen in Windows XP. I’m not sure
                        //       how to fix this yet, so I’m leaving the implementation as it is for now.
                        onReleased: {
                            backend.activateWindow(model.windowId)
                        }
                    }

                }
            }
        }

        Image{
            id: separator
            anchors.right: notificationAreaBackground.left
            anchors.bottom: notificationAreaBackground.bottom
            height: notificationAreaBackground.height
            width: 10
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/taskpanel_separator.bmp"
            fillMode: Image.TileHorizontally
        }

        Image{
            id: notificationAreaBackground
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height
            width: 104
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/clock_background.bmp"
            fillMode: Image.TileHorizontally

            Image{
                id: speakerButton
                anchors.right: dateTimeDisplay.left
                anchors.rightMargin: 11
                anchors.verticalCenter: parent.verticalCenter
                width:16
                height:16
                source: "file:/" + HOME + "/Bonsai/themes/windows_xp/taskbar/notification_area/speaker.bmp"
                fillMode: Image.Stretch

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        systemVolumeSlidrer.visible = true
                    }
                }

            }


            //Dodaj Text dla daty i godziny
            Text {
                id: dateTimeDisplay
                anchors.right: parent.right
                anchors.rightMargin: 17
                anchors.verticalCenter: parent.verticalCenter
                color: "white"

                font.family: "Tahoma"
                font.styleName: "Regular"
                font.pointSize: 8.0
                font.kerning: true
                font.hintingPreference: Font.PreferFullHinting
                renderType: Text.NativeRendering
                antialiasing: false

                text: Qt.formatDateTime(new Date(), "hh:mm")
                //text: Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss")

                // Timer do aktualizacji co sekundę
                Timer {
                    interval: 10000 // Odświeżanie co 10 sekund
                    running: true
                    repeat: true
                    onTriggered: {
                        //dateTimeDisplay.text = Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss")
                        dateTimeDisplay.text = Qt.formatDateTime(new Date(), "hh:mm")
                    }
                }
            }
        }

    }

    // ShapeTest{
    //import "file:/" + HOME + "/Bonsai/themes/windows_xp/"
    //      test qml component form filesystem
    //     x:0
    //  r4e   y:0
    //     visible: true
    // }

    SystemVolumeSlider{
        id: systemVolumeSlidrer
        x: panel.x + panel.width - width - 20
        y: panel.y-height

        visible: false

    }


    Button{
        anchors.top: parent.top
        anchors.right: parent.right
        text: "Gnome"
        onClicked: {
            backend.setActiveFrontend("Gnome")
        }
    }

    // ListView {
    //     id: themesList
    //     anchors.top: parent.top
    //     anchors.right: parent.right
    //     width: 200
    //     height: 200
    //     clip: true
    //     model: backend.themesModel

    //     delegate: Rectangle {
    //         required property string themeId
    //         required property string themeName
    //         required property bool themeActive
    //         height: 50
    //         width: themesList.width
    //         border.width: 1

    //         Row{
    //             anchors.fill: parent
    //             RadioButton{
    //                 checked: themeActive
    //                 onClicked: {
    //                     //backend.themesModel.setActiveTheme(themeId)
    //                     backend.setActiveTheme(themeId)
    //                 }
    //             }
    //             Text{
    //                 text: themeName
    //             }
    //         }
    //     }
    // }
}
