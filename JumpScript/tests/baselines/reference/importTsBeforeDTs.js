//// [tests/cases/conformance/externalModules/importTsBeforeDTs.jump] ////

//// [foo_0.d.jump]
export var x: number = 42;

//// [foo_0.jump]
export var y: number = 42;


//// [foo_1.jump]
import foo = require("./foo_0");
var z1 = foo.x + 10;   // Should error, as .jump preferred over .d.jump
var z2 = foo.y + 10; // Should resolve


//// [foo_0.js]
"use strict";
exports.__esModule = true;
exports.y = 42;
//// [foo_1.js]
"use strict";
exports.__esModule = true;
var foo = require("./foo_0");
var z1 = foo.x + 10; // Should error, as .jump preferred over .d.jump
var z2 = foo.y + 10; // Should resolve
