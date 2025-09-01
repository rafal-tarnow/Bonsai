import QtQuick
import Bonsai.Backend
import "../contents"

Window {
    id: root
    //flags:  Qt.Popup
    flags: Qt.Window | Qt.FramelessWindowHint
    color: "pink"
    visible: true
    height: 100
    width: 100

    modality: Qt.NonModal

    // Component.onCompleted: {
    //     BWindowManagerX11.hideFromTaskbar(root, true);
    //     BWindowManagerX11.hideFromSwitcher(root, true);
    //     BWindowManagerX11.hideFromPager(root, true);
    // }

    Timer {
        id: initTimer
        interval: 1 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {
            BWindowManagerX11.hideFromTaskbar(root, true);
            BWindowManagerX11.hideFromSwitcher(root, true);
            BWindowManagerX11.hideFromPager(root, true);
        }
    }

    AppLauncherContent{
        anchors.fill: parent
        onAppLaunched: {
            root.visible = false
        }
    }
}
