qt_standard_project_setup(REQUIRES ${REQUIRED_QT_VERSION})

qt_add_library(XPFrontend STATIC)

qt_add_qml_module(XPFrontend
    URI XPFrontend
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES TaskbarButton.qml
    QML_FILES TiledImage.qml
    QML_FILES AppMenuWindow.qml
    QML_FILES AppMenuContent.qml
    QML_FILES SystemVolumeSlider.qml
    QML_FILES TiledImageVertical.qml
    QML_FILES TurnOffWindow.qml
    QML_FILES LogOffWindow.qml
    QML_FILES common_components/XPMenu.qml
    QML_FILES common_components/XPMenuItem.qml
    QML_FILES common_components/XPMenuSeparator.qml
)



