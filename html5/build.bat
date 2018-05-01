@echo off

set CLOSURE_JAR=.\closure-compiler-v20180204.jar
set JSFILES=.\build\RadJav.js

cmd.exe /c tsc --importHelpers --removeComments --noImplicitUseStrict -p ./src/tsconfig.json --outDir build

copy .\build\*.js ..\library\javascript\

if not exist .\build\languages mkdir .\build\languages
if not exist .\build\themes mkdir .\build\themes
REM xcopy .\languages .\build\languages\ /Y /S /E
REM xcopy .\themes .\build\themes\ /Y /S /E

if exist %CLOSURE_JAR% (
	java -jar %CLOSURE_JAR% --js_output_file=.\build\RadJav.min.js %JSFILES%
)