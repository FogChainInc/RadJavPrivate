//// [tests/cases/compiler/nodeResolution3.jump] ////

//// [index.d.jump]
export var x: number;

//// [a.jump]
import y = require("b");

//// [a.js]
"use strict";
exports.__esModule = true;
