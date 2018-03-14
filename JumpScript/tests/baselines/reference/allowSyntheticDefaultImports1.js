//// [tests/cases/compiler/allowSyntheticDefaultImports1.jump] ////

//// [a.jump]
import Namespace from "./b";
export var x = new Namespace.Foo();

//// [b.d.jump]
export class Foo {
	member: string;
}


//// [a.js]
"use strict";
exports.__esModule = true;
var b_1 = require("./b");
exports.x = new b_1["default"].Foo();
