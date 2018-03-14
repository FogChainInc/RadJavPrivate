//// [tests/cases/compiler/allowSyntheticDefaultImports9.jump] ////

//// [b.d.jump]
export function foo();

export function bar();

//// [a.jump]
import { default as Foo } from "./b";
Foo.bar();
Foo.foo();

//// [a.js]
"use strict";
exports.__esModule = true;
var b_1 = require("./b");
b_1["default"].bar();
b_1["default"].foo();
