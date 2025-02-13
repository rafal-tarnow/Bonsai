cmake_minimum_required(VERSION 3.16)

# set(KF6Config_DIR /home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6Config)
# set(KF6WindowSystem_DIR /home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake/KF6WindowSystem)
set(CMAKE_PREFIX_PATH "/home/rafal/kde/usr/lib/x86_64-linux-gnu/cmake;${CMAKE_PREFIX_PATH}")
set(CMAKE_PREFIX_PATH "/home/rafal/kde/usr/lib/cmake;${CMAKE_PREFIX_PATH}")


set(BONSAI_VERSION 1.0.0)
project(Bonsai VERSION ${BONSAI_VERSION} LANGUAGES CXX)

set(QT_MAJOR_VERSION 6)

set(REQUIRED_QT_VERSION 6.8)
set(REQUIRED_KF_VERSION 6.2.0)
set(REQUIRED_PULSEAUDIOQT_VERSION 1.6.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(ECM ${REQUIRED_KF_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(ECMSourceVersionControl)
include(ECMSetupVersion)

if(${ECM_SOURCE_UNDER_VERSION_CONTROL})
    execute_process(COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Get the latest abbreviated commit hash of the working branch
    execute_process(COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
    add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

else()
    add_definitions(-DGIT_COMMIT_HASH="${BONSAI_VERSION}")
    add_definitions(-DGIT_BRANCH="Stable")
endif()

ecm_setup_version(${BONSAI_VERSION}
    VARIABLE_PREFIX BONSAI
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/bonsai_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/BonsaiConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR})

add_subdirectory(third_party/plasma-pa/)
add_subdirectory(BonsaiWallpaper/)
add_subdirectory(Bonsai/Backend/)
add_subdirectory(Bonsai/Wallpaper/)




feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

