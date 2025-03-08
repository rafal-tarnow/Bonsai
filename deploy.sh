#!/bin/bash

# Script usage: ./deploy.sh [/path/to/qt/qmake] [/path/to/project/qml/dir]
# Example: ./deploy.sh ~/Qt/Qt_6_8_2/6.8.2/gcc_64/bin/qmake ~/Pobrane/Bonsai/BonsaiWallpaper/XPFrontend

#### STEP 1 - Set up the Qt qmake path and QML directory
# Use the provided qmake path as the first argument, or default to a specific Qt installation
QMAKE_PATH=${1:-~/Qt/Qt_6_8_2/6.8.2/gcc_64/bin/qmake}

# Use the provided QML directory as the second argument, or default to ~/Pobrane/Bonsai/BonsaiWallpaper/XPFrontend
PROJECT_QML_DIR=${2:-~/Pobrane/Bonsai/BonsaiWallpaper/XPFrontend}

# Run cqtdeployer to deploy the app with specified binary, QML directory, and qmake path
cqtdeployer -bin appBonsaiWallpaper -qmlDir "$PROJECT_QML_DIR" -qmake "$QMAKE_PATH" force-clear deploySystem

#### STEP 2 - Gather and copy dependent libraries using lddtree
TARGET_DIR="./DistributionKit/lib/lddtree"
FINAL_DIR="./DistributionKit/lib"

# Create target directories if they don’t exist
mkdir -p "$TARGET_DIR"
mkdir -p "$FINAL_DIR"

# Get the list of libraries using lddtree and copy them to the target directory
lddtree -l appBonsaiWallpaper | while read LIB; do
    if [ -f "$LIB" ]; then
        cp -u "$LIB" "$TARGET_DIR"
    fi
done

#### STEP 3 - Copy specific libraries to ./DistributionKit/lib
# List of libraries to copy
LIBS_TO_COPY=(
    "libBonsaiBackend.so"
    "libplasma-volume.so.6"
    "libKF6ConfigCore.so.6"
    "libKF6WindowSystem.so.6"
    "libKF6PulseAudioQt.so.5"
    "libKF6I18n.so.6"
    "libKF6StatusNotifierItem.so.6"
    "libKF6ConfigGui.so.6"
    "libQt6Widgets.so.6"
)

# Copy the listed libraries from TARGET_DIR to FINAL_DIR
for LIB in "${LIBS_TO_COPY[@]}"; do
    SOURCE_PATH="$TARGET_DIR/$LIB"
    if [ -f "$SOURCE_PATH" ]; then
        cp -u "$SOURCE_PATH" "$FINAL_DIR"
        echo "Copied: $LIB to $FINAL_DIR"
    else
        echo "Not found: $LIB in $TARGET_DIR"
    fi
done

#### STEP 4 - Copy libxcb-cursor.so.0 from systemLibs to FINAL_DIR
# Define the source and destination for libxcb-cursor.so.0
XCB_CURSOR_SOURCE="./DistributionKit/lib/systemLibs/libxcb-cursor.so.0"
XCB_CURSOR_DEST="$FINAL_DIR"

# Check if the source file exists and copy it
if [ -f "$XCB_CURSOR_SOURCE" ]; then
    cp -u "$XCB_CURSOR_SOURCE" "$XCB_CURSOR_DEST"
    echo "Copied: libxcb-cursor.so.0 to $FINAL_DIR"
else
    echo "Not found: libxcb-cursor.so.0 in ./DistributionKit/lib/systemLibs"
fi
