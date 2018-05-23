#!/bin/sh

export WINEPREFIX=/opt/wine64
export NSIS="$WINEPREFIX/drive_c/Program Files (x86)/NSIS/makensis.exe"

mkdir -p ./build/RadJavVM/examples
mkdir -p ./build/RadJavVM/html5

cp -Rfu ../../../build-x64/Release ./build/RadJavVM
cp -Rfu ../../../../examples ./build/RadJavVM/examples
cp -Rfu ../../../../html5/build ./build/RadJavVM/html5
cp ../../../../LICENSE ./build/RadJavVM/LICENSE
cp ../../../../LICENSE.rtf ./build/RadJavVM/LICENSE.rtf

wine "${NSIS}/makensis.exe" /DPRODUCT_ARCH=x64 installer.nsi