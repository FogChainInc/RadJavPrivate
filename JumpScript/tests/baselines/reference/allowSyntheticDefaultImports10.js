//// [tests/cases/compiler/allowSyntheticDefaultImports10.jump] ////

//// [b.d.jump]
export function foo();

export function bar();

//// [a.jump]
import Foo = require("./b");
Foo.default.bar();
Foo.default.default.foo();

//// [a.js]
"use strict";
exports.__esModule = true;
var Foo = require("./b");
Foo["default"].bar();
Foo["default"]["default"].foo();
