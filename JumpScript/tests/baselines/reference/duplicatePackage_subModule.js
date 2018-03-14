//// [tests/cases/compiler/duplicatePackage_subModule.jump] ////

//// [index.d.jump]
import Foo from "foo/Foo";
export const o: Foo;

//// [Foo.d.jump]
export default class Foo {
    protected source: boolean;
}

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [Foo.d.jump]
export default class Foo {
    protected source: boolean;
}

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [index.jump]
import Foo from "foo/Foo";
import * as a from "a";

const o: Foo = a.o;


//// [index.js]
"use strict";
exports.__esModule = true;
var a = require("a");
var o = a.o;
