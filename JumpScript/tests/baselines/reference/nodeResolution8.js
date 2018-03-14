//// [tests/cases/compiler/nodeResolution8.jump] ////

//// [ref.jump]
var x = 1;

//// [index.d.jump]
/// <reference path="ref.jump"/>
export declare var y;


//// [b.jump]
import y = require("a");

//// [b.js]
"use strict";
exports.__esModule = true;
