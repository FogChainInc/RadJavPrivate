@echo off

set TYPEDOC_BIN=typedoc
set JS_DIR=..\html5\src
set THEME_PATH=..\html5\node_modules\typedoc-clarity-theme\bin

echo Generating Javascript documentation...

%TYPEDOC_BIN% --mode file --ignoreCompilerErrors --readme ./JSMessage.md --theme %THEME_PATH% --entryPoint RadJav --name RadJav --out js-docs %JS_DIR%

echo Finished Javascript documentation generation.