//// [tests/cases/compiler/duplicatePackage_referenceTypes.jump] ////

//// [index.d.jump]
/// <reference types="foo" />
import { Foo } from "foo";
export const foo: Foo;

//// [index.d.jump]
export class Foo { private x; }

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [index.d.jump]
export class Foo { private x; }

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [index.jump]
import * as a from "a";
import { Foo } from "foo";

let foo: Foo = a.foo;


//// [index.js]
"use strict";
exports.__esModule = true;
var a = require("a");
var foo = a.foo;
