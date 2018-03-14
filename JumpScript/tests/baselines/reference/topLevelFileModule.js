//// [tests/cases/conformance/externalModules/topLevelFileModule.jump] ////

//// [foo_0.jump]
export var x: number;

//// [fum.d.jump]
export declare var y: number;

//// [foo_1.jump]
import foo = require("./vs/foo_0");
import fum = require("./vs/fum");
var z = foo.x + fum.y;


//// [foo_0.js]
"use strict";
exports.__esModule = true;
//// [foo_1.js]
"use strict";
exports.__esModule = true;
var foo = require("./vs/foo_0");
var fum = require("./vs/fum");
var z = foo.x + fum.y;
