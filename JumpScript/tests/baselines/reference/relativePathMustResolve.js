//// [tests/cases/conformance/externalModules/relativePathMustResolve.jump] ////

//// [foo_0.jump]
export var x = 42;

//// [foo_1.jump]
import foo = require('./test/foo');
var z = foo.x + 10;


//// [foo_1.js]
"use strict";
exports.__esModule = true;
var foo = require("./test/foo");
var z = foo.x + 10;
