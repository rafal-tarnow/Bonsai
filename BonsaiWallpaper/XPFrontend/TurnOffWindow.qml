import QtQuick
import QtQuick.Controls

Window {
    id: root
    //flags:  Qt.Popup //Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    //flags:  Qt.Popup | Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    //flags: Qt.Tool | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint


    color: "#00000000"

    Behavior on color {
        id: behaviorOnBackgroundColor
        ColorAnimation {
            duration: 5000
        }
    }

    onVisibilityChanged: {
        if (visibility === Window.FullScreen) {
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
        source: "file:/" + HOME + "/Bonsai/themes/windows_xp/turn_off_dialog.png"

        Image{
            id: turnOffButton
            x: 141
            y: 81
            width: 33
            height: 33
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_power_button_normal.png"

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    turnOffButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_power_button_pressed.png"

                }
                onReleased: {
                    if(containsMouse){
                        backend.poweroff()
                    }
                    turnOffButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_power_button_normal.png"
                }
            }
        }

        Image{
            id: restartButton
            x: 226
            y: 81
            width: 33
            height: 33
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_normal.png"

            MouseArea {
                anchors.fill: parent
                hoverEnabled:  true
                onEntered: {
                    if(!pressed)
                        restartButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_hover.png"
                }
                onExited: {
                    if(!pressed)
                        restartButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_normal.png"
                }
                onPressed: {
                    restartButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_pressed.png"
                }
                onReleased: {
                    if(containsMouse){
                        restartButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_hover.png"
                        backend.reboot()
                    }else{
                        restartButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_restart_button_normal.png"
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
            source: "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_normal.png"

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    if(!pressed)
                        cancelButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_hover.png"
                }
                onExited: {
                    if(!pressed)
                        cancelButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_normal.png"
                }
                onPressed: {
                    cancelButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_pressed.png"
                }
                onReleased: {
                    if(containsMouse){
                        cancelButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_hover.png"
                        root.visibility = Window.Hidden
                    }else{
                        cancelButton.source = "file:/" + HOME + "/Bonsai/themes/windows_xp/dialog_turn_off_cancel_button_normal.png"
                    }
                }
            }
        }
    }
}









