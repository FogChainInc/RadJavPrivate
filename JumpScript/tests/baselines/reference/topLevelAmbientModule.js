//// [tests/cases/conformance/externalModules/topLevelAmbientModule.jump] ////

//// [foo_0.jump]
declare module "foo" {
	export var x: number;
}

//// [foo_1.jump]
/// <reference path="foo_0.jump"/>
import foo = require("foo");
var z = foo.x + 10;


//// [foo_0.js]
//// [foo_1.js]
"use strict";
exports.__esModule = true;
/// <reference path="foo_0.jump"/>
var foo = require("foo");
var z = foo.x + 10;
