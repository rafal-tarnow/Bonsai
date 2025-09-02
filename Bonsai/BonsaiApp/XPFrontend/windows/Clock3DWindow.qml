import QtQuick
import QtQuick.Controls
import Bonsai.Backend
import "../contents"

Window {
    id: root
    title: "Clock3DWindow"
    width: 200
    height: 200
    visible: true

    flags: Qt.WA_TranslucentBackground | Qt.FramelessWindowHint | Qt.WindowStaysOnBottomHint

    color: "#00000000"

    Timer {
        interval: 1 //czekaj 3 sekund zeby upewnić sie ze kwin sie uruchomił, inicjuj efekty kwin
        running: true
        repeat: false
        onTriggered: {
            BWindowManagerX11.hideFromTaskbar(root, true);
            BWindowManagerX11.hideFromSwitcher(root, true);
            BWindowManagerX11.hideFromPager(root, true);

            backend.setX11WindowTypeAsToolbar(root)
        }
    }

    Clock3DContent{
        anchors.fill: parent
    }
}
