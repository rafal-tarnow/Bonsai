qt_standard_project_setup(REQUIRES ${REQUIRED_QT_VERSION})

qt_add_library(XPFrontend STATIC)

qt_add_qml_module(XPFrontend
    URI XPFrontend
    VERSION 1.0

    QML_FILES
    Main.qml
    TaskbarButton.qml
    TiledImage.qml
    AppMenuWindow.qml
    AppMenuContent.qml
    SystemVolumeSlider.qml
    TiledImageVertical.qml
    TurnOffWindow.qml
    LogOffWindow.qml
    common_components/XPMenu.qml
    common_components/XPMenuItem.qml
    common_components/XPMenuSeparator.qml

    RESOURCES
    XBoomer/alldesktops.svg
    XBoomer/close.svg
    XBoomer/decoration.svg
    XBoomer/LICENSE
    XBoomer/maximize.svg
    XBoomer/metadata.desktop
    XBoomer/minimize.svg
    XBoomer/restore.svg
    XBoomer/shade.svg
    XBoomer/XBoomerrc
)



