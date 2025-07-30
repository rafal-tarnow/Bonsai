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

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Gui Quick DBus Concurrent Scxml)
find_package(KF6 ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS Config WindowSystem)
find_package(KF6PulseAudioQt ${REQUIRED_PULSEAUDIOQT_VERSION} REQUIRED)


# Statecharts:
# qt6_add_statecharts(${PROJECT_NAME}
#     GuiManagerStateMachine.scxml
# )



qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_URI}
    VERSION 1.0

    SOURCES
    Backend.hpp
    Backend.cpp
    AppsListModel.hpp
    AppsListModel.cpp

    SOURCES BackendAppsIconsProvider.hpp
    SOURCES TaskbarIconProvider.hpp
    SOURCES TaskbarIconProvider.cpp

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
    SOURCES private/Mask.hpp
    SOURCES private/Mask.cpp
    SOURCES BMaskedItem.hpp
    SOURCES BMaskedItem.cpp
    SOURCES private/MaskAlgorithm.hpp
    SOURCES private/MaskAlgorithm.cpp
    SOURCES private/MaskTracker.hpp
    SOURCES private/MaskTracker.cpp
    SOURCES BProxyWindow.hpp
    SOURCES BProxyWindow.cpp
    SOURCES strut/Strut.hpp
    SOURCES strut/StrutManager.hpp
    SOURCES strut/StrutManager.cpp
    SOURCES BSessionManager.hpp
    SOURCES BSessionManager.cpp
    SOURCES server/SessionService.hpp
    SOURCES server/SessionService.cpp
    SOURCES server/Server.hpp
    SOURCES server/Server.cpp
    SOURCES BTaskbarModel.hpp
    SOURCES BTaskbarModel.cpp
    SOURCES BTaskbarBaseModel.hpp
    SOURCES BTaskbarBaseModel.cpp
    SOURCES BFrontendManager.hpp
    SOURCES BFrontendManager.cpp
    SOURCES server/FrontendManagerAPIService.hpp
    SOURCES server/FrontendManagerAPIService.cpp
    SOURCES private/ProxyWindowController.hpp
    SOURCES private/ProxyWindowController.cpp
    SOURCES private/ProxyWindowServer.cpp
    SOURCES private/ProxyWindowServer.hpp
    SOURCES server/X11WindowManagerService.hpp
    SOURCES server/X11WindowManagerService.cpp
    SOURCES BWindowManagerX11.hpp
    SOURCES BWindowManagerX11.cpp
    SOURCES server/QmlGui.hpp
    SOURCES server/QmlGui.cpp
    SOURCES server/GuiManager.hpp
    SOURCES server/GuiManager.cpp
)

qt6_add_statecharts(${PROJECT_NAME}
    scxml/server_state_machine.scxml
    scxml/GuiManagerStateMachine.scxml
    #OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scxml
)

add_library(BonsaiBackendLib STATIC "")


#so that in the main application it would be possible to use headers, e.g., #include <Backend.hpp>
target_include_directories(BonsaiBackendLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::DBus
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Concurrent
    PUBLIC Qt6::Scxml
    PRIVATE KF6::ConfigCore
    PRIVATE KF6::WindowSystem
    PUBLIC KF6::PulseAudioQt
    # PkgConfig::LIBPULSE
    # PkgConfig::LIBPULSE_MAINLOOP
)
target_link_libraries(BonsaiBackendLib PRIVATE BonsaiBackend)
target_link_libraries(BonsaiBackendLib PRIVATE plasma-volume)

install(TARGETS ${PROJECT_NAME} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY assets/wallpapers/ DESTINATION ${KDE_INSTALL_WALLPAPERDIR}/Cask)
