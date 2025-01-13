import QtQuick
import QtQuick.Effects
import QtQuick.Controls


Window {
    id: root
    flags:  Qt.Popup //Qt.WindowStaysOnTopHint | Qt.WA_TranslucentBackground | Qt.FramelessWindowHint
    //flags: Qt.Tool | Qt.WindowStaysOnTopHint
    //flags: Qt.WindowStaysOnTopHint

    width: 380
    height: 478
    color: "#00000000"

    onVisibleChanged:{
        if(root.visible === false){
            appMenuContent.resetState()
        }
    }

    AppMenuContent{
        id: appMenuContent
        onHideRequest:{
            root.visible = false
            appMenuContent.resetState()
        }
    }
}

