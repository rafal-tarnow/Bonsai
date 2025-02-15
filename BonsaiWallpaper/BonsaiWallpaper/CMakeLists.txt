qt_standard_project_setup(REQUIRES 6.8)

qt_add_library(BonsaiWallpaper STATIC)

qt_add_qml_module(BonsaiWallpaper
    URI BonsaiWallpaper
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



