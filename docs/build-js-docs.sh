#!/bin/bash

TYPEDOC_BIN=typedoc
JS_DIR=../html5/src
THEME_PATH=../html5/node_modules/typedoc-clarity-theme/bin

echo >&2 "Generating Javascript documentation...";

$TYPEDOC_BIN --mode file --ignoreCompilerErrors --readme ./JSMessage.md --theme $THEME_PATH --out js-docs $JS_DIR

echo >&2 "Finished Javascript documentation generation.";