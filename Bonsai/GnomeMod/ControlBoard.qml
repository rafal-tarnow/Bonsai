import QtQuick.Controls.Material
//import QtQuick.Controls.Fusion
//import QtQuick.Controls.Imagine
//import QtQuick.Controls.Universal
//import QtQuick.Controls.FluentWinUI3

import QtQuick
import QtQuick.Controls
import "./common_qml"

Popup{
    id: root
    //flags:  Qt.Popup
    //color: "red"
    width: 370
    height: 327
    padding: 0
    topPadding: 19
    bottomPadding: 19
    leftPadding: 19
    rightPadding: 19

    popupType: Popup.Window
    
    //    Material.theme: Material.Light
    //   Material.accent: "#03a9f4"
    //   Material.primary: "#03a9f4"
    //   Material.background: Material.Teal
    //   Material.foreground: Material.Pink

    

    // Component.onCompleted: {
    //     backend.setX11WindowTypeAsDock(root)
    // }

    Material.primary: "#03a9f4"
    //Material.accent: "#03a9f4"
    //Material.background: "#ffffff"
    Material.accent: "#0073e5" //slider
    Material.background: "blue"
    Material.foreground:  "#3d3d3d" //text

    contentItem: Rectangle {
        color: "#FFFFFF"

        Column{
            anchors.fill: parent

            Column{
                width: parent.width
                spacing: 7
                Rectangle {
                    height: 37
                    anchors.left: parent.left
                    anchors.right: parent.right
                    //color: "#55546234"

                    Button{
                        text: "100%"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        height: parent.height + 12
                        width: 86
                        leftPadding:0
                        rightPadding: 0
                        icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/battery-level-100-symbolic.svg"
                        icon.color: UIStyle.colorButtonBatteryLevelIcon
                        icon.width: 15
                        icon.height: 15
                        Component.onCompleted: {
                            background.color = UIStyle.colorButtonBatteryLevel
                            contentItem.color = UIStyle.colorButtonBatteryLevelIcon
                            contentItem.font.pointSize = UIStyle.pointSizeControlBoardFont
                            contentItem.font.family = UIStyle.familyControlBoardFont
                        }
                    }

                    Row{
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: -6
                        spacing: 12

                        Button {
                            icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/screenshot-app-symbolic.svg"
                            icon.color: UIStyle.colorButtonScreenshotIcon
                            icon.width: 15
                            icon.height: 15
                            leftPadding:0
                            rightPadding: 0
                            height: parent.height + 12
                            width: height - 12
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                background.color = UIStyle.colorButtonScreenshot
                            }
                        }

                        Button {
                            icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/system-settings-symbolic.svg"
                            icon.color: UIStyle.colorButtonSystemSettingsIcon
                            icon.width: 15
                            icon.height: 15
                            leftPadding:0
                            rightPadding: 0
                            height: parent.height + 12
                            width: height - 12
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                background.color = UIStyle.colorButtonSystemSettings
                            }
                        }

                        Button {
                            icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/system-lock-screen-symbolic.svg"
                            icon.color: UIStyle.colorButtonLockIcon
                            icon.width: 15
                            icon.height: 15
                            leftPadding:0
                            rightPadding: 0
                            height: parent.height + 12
                            width: height - 12
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                background.color = UIStyle.colorButtonLock
                            }
                        }

                        Button {
                            icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/system-shutdown-symbolic.svg"
                            icon.color: UIStyle.colorButtonPowerIcon
                            icon.width: 15
                            icon.height: 15
                            leftPadding:0
                            rightPadding: 0
                            height: parent.height + 12
                            width: height - 12
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                background.color = UIStyle.colorButtonPower
                            }
                        }
                    }
                }
                Rectangle {
                    height: 37
                    anchors.left: parent.left
                    anchors.right: parent.right
                    //color: "#55889900"
                    RoundButton{
                        id: spekaerButton
                        height: parent.height + 5
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: -6
                        icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/audio-volume-high-symbolic.svg"
                        icon.color: UIStyle.colorControlBoardIcon
                        display: AbstractButton.IconOnly

                        background: Rectangle{
                            radius: spekaerButton.height/2
                            color: spekaerButton.hovered ? "#e8e8e8" : "#FFFFFF"
                        }
                    }

                    Slider{
                        from: 0
                        value: (audioBackend.prefferedOutputVolume / audioBackend.normalVolume * 100)
                        to: 100
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: spekaerButton.right
                        onMoved: {
                            audioBackend.prefferedOutputVolume = value * audioBackend.normalVolume / 100
                        }
                    }

                }
                Rectangle {
                    height: 37
                    anchors.left: parent.left
                    anchors.right: parent.right
                    //color: "#55881100"
                    RoundButton{
                        id: brightness
                        height: parent.height + 5
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: -6
                        icon.source: "file:" + HOME + "/Bonsai/themes/gnome/icons/display-brightness-symbolic.svg"
                        icon.color: UIStyle.colorControlBoardIcon
                        display: AbstractButton.IconOnly

                        background: Rectangle{
                            radius: brightness.height/2
                            color: brightness.hovered ? "#e8e8e8" : "#FFFFFF"
                        }
                    }

                    Slider{
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: brightness.right
                    }
                }

                Column{
                    width: parent.width
                    topPadding: 1
                    spacing: 12
                    Rectangle {
                        height: 44
                        anchors.left: parent.left
                        anchors.right: parent.right
                        //color: "#558800BB"
                        Row{
                            anchors.fill: parent
                            spacing: 12
                            ActionButton{
                                width: 160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                leftText: "Wi-Fi"
                                leftIconSource: "file:" + HOME + "/Bonsai/themes/gnome/images/network-wireless-connected-symbolic.svg"
                                iconColorActive: UIStyle.colorControlBoardIconActive
                                iconColorInactive: UIStyle.colorControlBoardIcon
                            }

                            // Button{
                            //     width: 160
                            //     height: parent.height + 12
                            //     anchors.verticalCenter: parent.verticalCenter
                            // }
                            ActionButton{
                                width:160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                leftText: "Bluetooth"
                                leftIconSource: "file:" + HOME + "/Bonsai/themes/gnome/icons/bluetooth-active-symbolic.svg"
                                iconColorActive: UIStyle.colorControlBoardIconActive
                                iconColorInactive: UIStyle.colorControlBoardIcon
                            }
                        }
                    }


                    Rectangle {
                        height: 44
                        anchors.left: parent.left
                        anchors.right: parent.right
                        //color: "#55149900"
                        Row{
                            anchors.fill: parent
                            spacing: 12
                            ActionButton{
                                width: 160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                leftText: "Power Profile"
                                leftIconSource: "file:" + HOME + "/Bonsai/themes/gnome/images/network-wireless-connected-symbolic.svg"
                                iconColorActive: UIStyle.colorControlBoardIconActive
                                iconColorInactive: UIStyle.colorControlBoardIcon
                            }
                            Button{
                                width:160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Night Mode")
                                Component.onCompleted: {
                                    background.color = UIStyle.colorButtonBatteryLevel
                                    contentItem.color = UIStyle.colorButtonBatteryLevelIcon
                                    contentItem.font.pointSize = UIStyle.pointSizeControlBoardFont
                                    contentItem.font.family = UIStyle.familyControlBoardFont
                                }
                            }
                        }
                    }
                    Rectangle {
                        height: 44
                        anchors.left: parent.left
                        anchors.right: parent.right
                        //color: "#55881155"
                        Row{
                            anchors.fill: parent
                            spacing: 12
                            Button{
                                width: 160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Dark Style")
                                Component.onCompleted: {
                                    background.color = UIStyle.colorButtonBatteryLevel
                                    contentItem.color = UIStyle.colorButtonBatteryLevelIcon
                                    contentItem.font.pointSize = UIStyle.pointSizeControlBoardFont
                                    contentItem.font.family = UIStyle.familyControlBoardFont
                                }
                            }
                            Button{
                                width:160
                                height: parent.height + 12
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Airplane Mode")
                                Component.onCompleted: {
                                    background.color = UIStyle.colorButtonBatteryLevel
                                    contentItem.color = UIStyle.colorButtonBatteryLevelIcon
                                    contentItem.font.pointSize = UIStyle.pointSizeControlBoardFont
                                    contentItem.font.family = UIStyle.familyControlBoardFont
                                }
                            }
                        }
                    }
                }
            }
        }




    }


    background: Rectangle{
        //color: "#fafafa"
        color: "#FFFFFF"
        anchors.fill: parent
        border.width: 1
        border.color: "#dedede"
        radius: 15
        

    }
}
