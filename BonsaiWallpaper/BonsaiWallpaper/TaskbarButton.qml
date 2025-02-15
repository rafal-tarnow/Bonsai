import QtQuick

Item{

    property alias text: buttonText.text
    property alias source: icon.source
    property int minimumWidth: normalButtonImage.minimumWidth
    property int minimumHeight: normalButtonImage.minimumHeight

    enum ButtonState {
        Normal,
        NormalHover,
        Pressed,
        PressedHover
    }

    property int buttonState: TaskbarButton.ButtonState.Normal

    onHeightChanged: {
        if(height < minimumHeight){
            height = minimumHeight
        }
    }

    onWidthChanged: {
        if(width < minimumWidth){
            width = minimumWidth
        }
    }

    TiledImage{
        id: normalButtonImage
        width: parent.width
        height: parent.height
        visible: parent.buttonState === TaskbarButton.ButtonState.Normal

        leftTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/top_left.png"
        leftMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/middle_left.png"
        leftBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/bottom_left.png"

        middleTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/top_middle.png"
        middleMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/middle_middle.bmp"
        middleBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/bottom_middle.png"

        rightTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/top_right.png"
        rightMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/middle_right.png"
        rightBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_normal/bottom_right.png"
    }

    TiledImage{
        id: hoverButtonImage
        width: parent.width
        height: parent.height
        visible: parent.buttonState === TaskbarButton.ButtonState.NormalHover

        leftTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/left_top.bmp"
        leftMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/left_middle.bmp"
        leftBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/left_bottom.bmp"

        middleTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/middle_top.bmp"
        middleMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/middle_middle.bmp"
        middleBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/middle_bottom.bmp"

        rightTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/right_top.bmp"
        rightMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/right_middle.bmp"
        rightBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_hover/right_bottom.bmp"
    }

    TiledImage{
        id: pressedButtonImage
        width: parent.width
        height: parent.height
        visible: parent.buttonState === TaskbarButton.ButtonState.Pressed

        leftTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/left_top.bmp"
        leftMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/left_middle.bmp"
        leftBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/left_bottom.bmp"

        middleTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/middle_top.bmp"
        middleMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/middle_middle.bmp"
        middleBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/middle_bottom.bmp"

        rightTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/right_top.bmp"
        rightMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/right_middle.bmp"
        rightBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed/right_bottom.bmp"
    }

    TiledImage{
        id: pressedHoverButtonImage
        width: parent.width
        height: parent.height
        visible: parent.buttonState === TaskbarButton.ButtonState.PressedHover

        leftTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/left_top.bmp"
        leftMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/left_middle.bmp"
        leftBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/left_bottom.bmp"

        middleTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/middle_top.bmp"
        middleMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/middle_middle.bmp"
        middleBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/middle_bottom.bmp"

        rightTopSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/right_top.bmp"
        rightMiddleSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/right_middle.bmp"
        rightBottomSource: "file:/home/rafal/Bonsai/themes/windows_xp/taskbar/background_button_pressed_hover/right_bottom.bmp"
    }

    Image{
        id: icon
        width: 16
        height: 16
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.leftMargin: 10
        anchors.left: parent.left
        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
    }

    Text{
        id: buttonText
        anchors.left: icon.right
        anchors.leftMargin: 5
        anchors.verticalCenter : parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: "My Computer"
        color: "white"

        font.family: "Tahoma"
        font.styleName: "Regular"
        font.pointSize: 8.0
        font.kerning: true
        font.hintingPreference: Font.PreferFullHinting
        renderType: Text.NativeRendering
        antialiasing: false

        clip: true
    }
}
