//// [tests/cases/conformance/externalModules/topLevelFileModuleMissing.jump] ////

//// [foo_0.jump]
export var x: number;

//// [foo_1.jump]
import foo = require("vs/foo");
var z = foo.x + 10;


//// [foo_1.js]
"use strict";
exports.__esModule = true;
var foo = require("vs/foo");
var z = foo.x + 10;
