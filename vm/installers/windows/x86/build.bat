@echo off

set NSIS=C:\Program Files (x86)\NSIS

if not exist .\build mkdir .\build
if not exist .\build\RadJavVM mkdir .\build\RadJavVM
if not exist .\build\RadJavVM\examples mkdir .\build\RadJavVM\examples
if not exist .\build\RadJavVM\html5 mkdir .\build\RadJavVM\html5

xcopy ..\..\..\build-x86\Release .\build\RadJavVM /Y /S /E
xcopy ..\..\..\..\examples .\build\RadJavVM\examples /Y /S /E
xcopy ..\..\..\..\html5\build .\build\RadJavVM\html5 /Y /S /E
copy ..\..\..\..\LICENSE .\build\RadJavVM\LICENSE
copy ..\..\..\..\LICENSE.rtf .\build\RadJavVM\LICENSE.rtf

"%NSIS%\makensis.exe" /DPRODUCT_ARCH=x86 ../installer.nsi