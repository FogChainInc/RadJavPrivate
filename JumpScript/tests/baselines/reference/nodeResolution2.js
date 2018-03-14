//// [tests/cases/compiler/nodeResolution2.jump] ////

//// [a.d.jump]
export var x: number;

//// [b.jump]
import y = require("a");

//// [b.js]
"use strict";
exports.__esModule = true;
