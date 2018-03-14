//// [tests/cases/compiler/moduleAugmentationGlobal2.jump] ////

//// [f1.jump]
export class A {};
//// [f2.jump]
// change the shape of Array<T>
import {A} from "./f1";

declare global {
    interface Array<T> {
        getCountAsString(): string;
    }
}

let x = [1];
let y = x.getCountAsString().toLowerCase();


//// [f1.js]
"use strict";
exports.__esModule = true;
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
exports.A = A;
;
//// [f2.js]
"use strict";
exports.__esModule = true;
var x = [1];
var y = x.getCountAsString().toLowerCase();


//// [f1.d.jump]
export declare class A {
}
//// [f2.d.jump]
declare global  {
    interface Array<T> {
        getCountAsString(): string;
    }
}
export {};
