#!/bin/sh

CLOSURE_JAR=.\closure-compiler-v20180204.jar
JSFILES=.\build\RadJav.js

tsc --importHelpers --removeComments --noImplicitUseStrict -p ./src/tsconfig.json --outDir build || true

cp .\build\*.js ..\library\javascript\

mkdir -p .\build\languages
mkdir -p .\build\themes

cp -Rfu.\languages .\build\languages\
cp -Rfu .\themes .\build\themes\

if exist %CLOSURE_JAR% (
	java -jar %CLOSURE_JAR% --js_output_file=.\build\RadJav.min.js %JSFILES%
)