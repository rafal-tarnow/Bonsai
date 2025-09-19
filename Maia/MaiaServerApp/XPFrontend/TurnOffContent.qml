import QtQuick
import QtQuick.Controls
import Maia.Client

Rectangle {
    id: root
    color: "#00000000"

    signal cancelClicked()

    Behavior on color {
        id: behaviorOnBackgroundColor
        ColorAnimation {
            duration: 5000
        }
    }

    onVisibleChanged: {
        if (visible === true) {
            behaviorOnBackgroundColor.enabled = true
            color = "#BB000000";
        } else {
            behaviorOnBackgroundColor.enabled = false
            color = "#00000000";
        }
    }

    Image{
        id: turnOffDialog
        anchors.centerIn: parent
        width: 314
        height: 200
        source: "qrc:/assets/images/turn_off_dialog.png"

        Image{
            id: turnOffButton
            x: 141
            y: 81
            width: 33
            height: 33
            source: "qrc:/assets/images/dialog_turn_off_power_button_normal.png"

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    turnOffButton.source = "qrc:/assets/images/dialog_turn_off_power_button_pressed.png"

                }
                onReleased: {
                    if(containsMouse){
                        BSessionManager.poweroff()
                    }
                    turnOffButton.source = "qrc:/assets/images/dialog_turn_off_power_button_normal.png"
                }
            }
        }

        Image{
            id: restartButton
            x: 226
            y: 81
            width: 33
            height: 33
            source: "qrc:/assets/images/dialog_turn_off_restart_button_normal.png"

            MouseArea {
                anchors.fill: parent
                hoverEnabled:  true
                onEntered: {
                    if(!pressed)
                        restartButton.source = "qrc:/assets/images/dialog_turn_off_restart_button_hover.png"
                }
                onExited: {
                    if(!pressed)
                        restartButton.source = "qrc:/assets/images/dialog_turn_off_restart_button_normal.png"
                }
                onPressed: {
                    restartButton.source = "qrc:/assets/images/dialog_turn_off_restart_button_pressed.png"
                }
                onReleased: {
                    if(containsMouse){
                        restartButton.source = "qrc:/assets/images/dialog_turn_off_restart_button_hover.png"
                        BSessionManager.reboot()
                    }else{
                        restartButton.source = "qrc:/assets/images/dialog_turn_off_restart_button_normal.png"
                    }
                }
            }
        }

        Image{
            id: cancelButton
            x: 243
            y: 168
            width: 61
            height: 21
            source: "qrc:/assets/images/dialog_turn_off_cancel_button_normal.png"

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    if(!pressed)
                        cancelButton.source = "qrc:/assets/images/dialog_turn_off_cancel_button_hover.png"
                }
                onExited: {
                    if(!pressed)
                        cancelButton.source = "qrc:/assets/images/dialog_turn_off_cancel_button_normal.png"
                }
                onPressed: {
                    cancelButton.source = "qrc:/assets/images/dialog_turn_off_cancel_button_pressed.png"
                }
                onReleased: {
                    if(containsMouse){
                        cancelButton.source = "qrc:/assets/images/dialog_turn_off_cancel_button_hover.png"
                        root.cancelClicked()
                    }else{
                        cancelButton.source = "qrc:/assets/images/dialog_turn_off_cancel_button_normal.png"
                    }
                }
            }
        }
    }
}
