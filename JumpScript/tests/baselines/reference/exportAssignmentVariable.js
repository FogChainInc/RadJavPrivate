//// [tests/cases/compiler/exportAssignmentVariable.jump] ////

//// [exportAssignmentVariable_A.jump]
var x = 0;

export = x;

//// [exportAssignmentVariable_B.jump]
import y = require("./exportAssignmentVariable_A");

var n: number = y;

//// [exportAssignmentVariable_A.js]
"use strict";
var x = 0;
module.exports = x;
//// [exportAssignmentVariable_B.js]
"use strict";
exports.__esModule = true;
var y = require("./exportAssignmentVariable_A");
var n = y;
