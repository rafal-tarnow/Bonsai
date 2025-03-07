project(BonsaiBackend VERSION ${BONSAI_VERSION})

set(PROJECT_URI Bonsai.Backend)
#set(QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMGenerateHeaders)
include(ECMFindQmlModule)
include(ECMQmlModule)
include(FindPkgConfig)
include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Gui Quick DBus Concurrent)
find_package(KF6 ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS Config WindowSystem)
find_package(KF6PulseAudioQt ${REQUIRED_PULSEAUDIOQT_VERSION} REQUIRED)


qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_URI}
    VERSION 1.0

    SOURCES
    Backend.hpp
    Backend.cpp
    AppsListModel.hpp
    AppsListModel.cpp

    SOURCES BackendAppsIconsProvider.hpp
    SOURCES Model.hpp
    SOURCES Model.cpp

    RESOURCES
    SOURCES AudioBackend.hpp
    SOURCES AudioBackend.cpp
    SOURCES FavoriteAppsListModel.hpp
    SOURCES FavoriteAppsListModel.cpp
    SOURCES ThemesModel.hpp
    SOURCES ThemesModel.cpp
    SOURCES WindowManagerDBus.hpp
    SOURCES WindowManagerDBus.cpp
    SOURCES helper/Process.hpp
    SOURCES helper/Process.cpp
)

add_library(BonsaiBackendLib STATIC "")

#so that in the main application it would be possible to use headers, e.g., #include <Backend.hpp>
target_include_directories(BonsaiBackendLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::DBus
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Quick
    Qt6::Concurrent
    KF6::ConfigCore
    KF6::WindowSystem
    PUBLIC KF6::PulseAudioQt
    # PkgConfig::LIBPULSE
    # PkgConfig::LIBPULSE_MAINLOOP
)
target_link_libraries(BonsaiBackendLib PRIVATE BonsaiBackend)
target_link_libraries(BonsaiBackendLib PRIVATE plasma-volume)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY assets/wallpapers/ DESTINATION ${KDE_INSTALL_WALLPAPERDIR}/Cask)
