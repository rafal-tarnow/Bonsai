#!/bin/bash

#script usage: ./deploy.sh /path/to/qt/qmake



#### STEP 1

QMAKE_PATH=${1:-~/Qt/Qt_6_8_2/6.8.2/gcc_64/bin/qmake}

cqtdeployer -bin appBonsaiWallpaper -qmlDir ~/Pobrane/Bonsai/BonsaiWallpaper/XPFrontend -qmake "$QMAKE_PATH" force-clear deploySystem


#### STEP 2

TARGET_DIR="./DistributionKit/lib/lddtree"

# Tworzenie katalogu docelowego, jeśli nie istnieje
#rm -r "$TARGET_DIR"
mkdir -p "$TARGET_DIR"

#Pobranie listy bibliotek za pomocą lddtree i skopiowanie ich do katalogu docelowego
lddtree -l appBonsaiWallpaper | while read LIB; do
    if [ -f "$LIB" ]; then
        cp -u "$LIB" "$TARGET_DIR"
    fi
done


#### STEP 3


#przekopiowa zawartosc ./DistributionKit/lib/systemLib do ./DistributionKit/lib

#### STEP 4

# usuna libc libpthread libm ld i std++ lib form ./DistributionKit/lib

#echo "Biblioteki zostały skopiowane do $TARGET_DIR"
