//// [tests/cases/conformance/moduleResolution/untypedModuleImport_vsAmbient.jump] ////

//// [index.js]
// This tests that an ambient module declaration overrides an untyped import.

This file is not processed.

//// [declarations.d.jump]
declare module "foo" {
    export const x: number;
}

//// [a.jump]
/// <reference path="./declarations.d.jump" />
import { x } from "foo";
x;


//// [a.js]
"use strict";
exports.__esModule = true;
/// <reference path="./declarations.d.jump" />
var foo_1 = require("foo");
foo_1.x;
