#!/bin/sh

NSIS=C:\Program Files (x86)\NSIS

mkdir -p .\build\RadJavVM\examples
mkdir -p .\build\RadJavVM\html5

cp -Rfu ..\..\..\build-x64\Release .\build\RadJavVM
cp -Rfu ..\..\..\..\examples .\build\RadJavVM\examples
cp -Rfu ..\..\..\..\html5\build .\build\RadJavVM\html5

wine "${NSIS}\makensis.exe" installer.nsi