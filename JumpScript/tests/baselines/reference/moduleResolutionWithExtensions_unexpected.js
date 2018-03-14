//// [tests/cases/compiler/moduleResolutionWithExtensions_unexpected.jump] ////

//// [normalize.css]
// This tests that a package.json "main" with an unexpected extension is ignored.

This file is not read.

//// [package.json]
{ "main": "normalize.css" }

//// [a.jump]
import "normalize.css";


//// [a.js]
"use strict";
exports.__esModule = true;
require("normalize.css");
