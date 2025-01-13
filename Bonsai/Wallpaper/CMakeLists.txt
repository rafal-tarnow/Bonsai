set(BACKEND_QML_MODULE "Bonsai.Wallpaper")
set(QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Qml Quick )

qt_standard_project_setup(REQUIRES 6.5)

qt_add_qml_module(BonsaiWallpaper
    URI Bonsai.Wallpaper
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


target_link_libraries(BonsaiWallpaper PRIVATE plasma-volume)


add_library(BonsaiWallpaperLib STATIC "")
target_link_libraries(BonsaiWallpaperLib PRIVATE BonsaiWallpaper)

