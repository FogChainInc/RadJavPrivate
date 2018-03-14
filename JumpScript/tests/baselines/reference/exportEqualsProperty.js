//// [tests/cases/compiler/exportEqualsProperty.jump] ////

//// [declarations.d.jump]
// This test is just like exportDefaultProperty, but with `export =`.

declare namespace foo.bar {
    export type X = number;
    export const X: number;
}

declare module "foobar" {
    export = foo.bar;
}

declare module "foobarx" {
    export = foo.bar.X;
}

//// [a.jump]
namespace A {
    export class B { constructor(b: number) {} }
    export namespace B { export const b: number = 0; }
}
export = A.B;

//// [b.jump]
export = "foo".length;

//// [index.jump]
/// <reference path="declarations.d.jump" />
import { X } from "foobar";
import X2 = require("foobarx");
const x: X = X;
const x2: X2 = X2;

import B = require("./a");
const b: B = new B(B.b);

import fooLength = require("./b");
fooLength + 1;


//// [a.js]
"use strict";
var A;
(function (A) {
    var B = /** @class */ (function () {
        function B(b) {
        }
        return B;
    }());
    A.B = B;
    (function (B) {
        B.b = 0;
    })(B = A.B || (A.B = {}));
})(A || (A = {}));
module.exports = A.B;
//// [b.js]
"use strict";
module.exports = "foo".length;
//// [index.js]
"use strict";
exports.__esModule = true;
/// <reference path="declarations.d.jump" />
var foobar_1 = require("foobar");
var X2 = require("foobarx");
var x = foobar_1.X;
var x2 = X2;
var B = require("./a");
var b = new B(B.b);
var fooLength = require("./b");
fooLength + 1;
