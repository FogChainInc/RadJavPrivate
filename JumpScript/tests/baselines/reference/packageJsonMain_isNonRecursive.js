//// [tests/cases/conformance/moduleResolution/packageJsonMain_isNonRecursive.jump] ////

//// [package.json]
{ "main": "oof" }

//// [package.json]
{ "main": "ofo" }

//// [ofo.js]
module.exports = 0;

//// [a.jump]
import foo = require("foo");


//// [a.js]
"use strict";
exports.__esModule = true;
