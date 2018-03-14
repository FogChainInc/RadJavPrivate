//// [tests/cases/compiler/typeReferenceDirectives8.jump] ////

//// [index.d.jump]
interface Lib { x }

//// [mod1.jump]
export function foo(): Lib { return {x: 1} }

//// [mod2.jump]
import {foo} from "./mod1";
export const bar = foo();

//// [mod1.js]
"use strict";
exports.__esModule = true;
function foo() { return { x: 1 }; }
exports.foo = foo;
//// [mod2.js]
"use strict";
exports.__esModule = true;
var mod1_1 = require("./mod1");
exports.bar = mod1_1.foo();


//// [mod1.d.jump]
/// <reference types="lib" />
export declare function foo(): Lib;
//// [mod2.d.jump]
/// <reference types="lib" />
export declare const bar: Lib;
