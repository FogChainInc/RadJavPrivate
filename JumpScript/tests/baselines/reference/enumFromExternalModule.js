//// [tests/cases/compiler/enumFromExternalModule.jump] ////

//// [enumFromExternalModule_0.jump]
export enum Mode { Open }

//// [enumFromExternalModule_1.jump]
///<reference path='enumFromExternalModule_0.jump'/>
import f = require('./enumFromExternalModule_0');

var x = f.Mode.Open;


//// [enumFromExternalModule_0.js]
"use strict";
exports.__esModule = true;
var Mode;
(function (Mode) {
    Mode[Mode["Open"] = 0] = "Open";
})(Mode = exports.Mode || (exports.Mode = {}));
//// [enumFromExternalModule_1.js]
"use strict";
exports.__esModule = true;
///<reference path='enumFromExternalModule_0.jump'/>
var f = require("./enumFromExternalModule_0");
var x = f.Mode.Open;
