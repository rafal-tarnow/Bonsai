cmake_minimum_required(VERSION 3.16)

project(BonsaiWallpaper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Quick Quick3D DBus WebEngineQuick)

qt_standard_project_setup(REQUIRES ${REQUIRED_QT_VERSION})

qt_add_executable(appBonsaiWallpaper
    main.cpp
    main_proxy_window.cpp
    main_server.cpp
    main_server.hpp
    main_proxy_window.hpp
    logger.hpp
    logger.cpp


)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appBonsaiWallpaper PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBonsaiWallpaper
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appBonsaiWallpaper
    PRIVATE Qt6::Quick
    PRIVATE Qt6::DBus
    PRIVATE Qt6::Quick3D
    PRIVATE Qt6::WebEngineQuick
)

target_link_libraries(appBonsaiWallpaper PRIVATE BonsaiBackend)
target_link_libraries(appBonsaiWallpaper PRIVATE XPFrontendplugin)
target_link_libraries(appBonsaiWallpaper PRIVATE plasma-volume)

include(GNUInstallDirs)
install(TARGETS appBonsaiWallpaper
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Generowanie skryptu do deployowania aplikacji Qt Quick
qt_generate_deploy_qml_app_script(
    TARGET appBonsaiWallpaper
    OUTPUT_SCRIPT deploy_script
)

# Instalacja skryptu (gdzie skrypt będzie uruchomiony podczas instalacji)
install(SCRIPT ${deploy_script})


add_subdirectory(./XPFrontend/)
