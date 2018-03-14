//// [tests/cases/conformance/externalModules/umd5.jump] ////

//// [foo.d.jump]
export var x: number;
export function fn(): void;
export interface Thing { n: typeof x }
export as namespace Foo;

//// [a.jump]
import * as Bar from './foo';
Bar.fn();
let x: Bar.Thing;
let y: number = x.n;
// should error
let z = Foo;


//// [a.js]
"use strict";
exports.__esModule = true;
var Bar = require("./foo");
Bar.fn();
var x;
var y = x.n;
// should error
var z = Foo;
