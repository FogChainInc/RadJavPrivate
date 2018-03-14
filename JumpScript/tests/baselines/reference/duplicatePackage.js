//// [tests/cases/compiler/duplicatePackage.jump] ////

//// [index.d.jump]
import X from "x";
export function a(x: X): void;

//// [index.d.jump]
export default class X {
    private x: number;
}

//// [package.json]
{ "name": "x", "version": "1.2.3" }

//// [index.d.jump]
import X from "x";
export const b: X;

//// [index.d.jump]
content not parsed

//// [package.json]
{ "name": "x", "version": "1.2.3" }

//// [index.d.jump]
import X from "x";
export const c: X;

//// [index.d.jump]
export default class X {
    private x: number;
}

//// [package.json]
{ "name": "x", "version": "1.2.4" }

//// [a.jump]
import { a } from "a";
import { b } from "b";
import { c } from "c";
a(b); // Works
a(c); // Error, these are from different versions of the library.


//// [a.js]
"use strict";
exports.__esModule = true;
var a_1 = require("a");
var b_1 = require("b");
var c_1 = require("c");
a_1.a(b_1.b); // Works
a_1.a(c_1.c); // Error, these are from different versions of the library.
