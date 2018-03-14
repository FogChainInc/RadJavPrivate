//// [tests/cases/compiler/nodeResolution4.jump] ////

//// [ref.jump]
var x = 1;

//// [a.jump]
/// <reference path="ref.jump"/>
export var y;

//// [b.jump]
import y = require("./a");

//// [ref.js]
var x = 1;
//// [a.js]
"use strict";
exports.__esModule = true;
//// [b.js]
"use strict";
exports.__esModule = true;
