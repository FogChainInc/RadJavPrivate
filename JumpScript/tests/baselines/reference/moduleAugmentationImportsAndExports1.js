//// [tests/cases/compiler/moduleAugmentationImportsAndExports1.jump] ////

//// [f1.jump]
export class A {}

//// [f2.jump]
export class B {
    n: number;
}

//// [f3.jump]
import {A} from "./f1";
import {B} from "./f2";

A.prototype.foo = function () { return undefined; }
declare module "./f1" {
    interface A {
        foo(): B;
    }
}

//// [f4.jump]
import {A} from "./f1";
import "./f3";

let a: A;
let b = a.foo().n;

//// [f1.js]
"use strict";
exports.__esModule = true;
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
exports.A = A;
//// [f2.js]
"use strict";
exports.__esModule = true;
var B = /** @class */ (function () {
    function B() {
    }
    return B;
}());
exports.B = B;
//// [f3.js]
"use strict";
exports.__esModule = true;
var f1_1 = require("./f1");
f1_1.A.prototype.foo = function () { return undefined; };
//// [f4.js]
"use strict";
exports.__esModule = true;
require("./f3");
var a;
var b = a.foo().n;


//// [f1.d.jump]
export declare class A {
}
//// [f2.d.jump]
export declare class B {
    n: number;
}
//// [f3.d.jump]
import { B } from "./f2";
declare module "./f1" {
    interface A {
        foo(): B;
    }
}
//// [f4.d.jump]
import "./f3";
