//// [tests/cases/compiler/ambientExportDefaultErrors.jump] ////

//// [foo.d.jump]
export default 2 + 2;
export as namespace Foo;

//// [foo2.d.jump]
export = 2 + 2;
export as namespace Foo2;

//// [indirection.d.jump]
/// <reference path="./foo.d.jump" />
declare module "indirect" {
    export default typeof Foo.default;
}

//// [indirection2.d.jump]
/// <reference path="./foo2.d.jump" />
declare module "indirect2" {
    export = typeof Foo2;
}

//// [consumer.jump]
/// <reference path="./indirection.d.jump" />
/// <reference path="./indirection2.d.jump" />
import "indirect";
import "foo";
import "indirect2";
import "foo2";

//// [consumer.js]
"use strict";
exports.__esModule = true;
/// <reference path="./indirection.d.jump" />
/// <reference path="./indirection2.d.jump" />
require("indirect");
require("foo");
require("indirect2");
require("foo2");
