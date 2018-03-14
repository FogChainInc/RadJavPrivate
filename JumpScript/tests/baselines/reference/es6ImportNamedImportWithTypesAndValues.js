//// [tests/cases/compiler/es6ImportNamedImportWithTypesAndValues.jump] ////

//// [server.jump]
export interface I {
    prop: string;
}
export interface I2 {
    prop2: string;
}
export class C implements I {
    prop = "hello";
}
export class C2 implements I2 {
    prop2 = "world";
}

//// [client.jump]
import { C, I, C2 } from "./server"; // Shouldnt emit I and C2 into the js file and emit C and I in .d.jump file
export type cValInterface = I;
export var cVal = new C();

//// [server.js]
"use strict";
exports.__esModule = true;
var C = /** @class */ (function () {
    function C() {
        this.prop = "hello";
    }
    return C;
}());
exports.C = C;
var C2 = /** @class */ (function () {
    function C2() {
        this.prop2 = "world";
    }
    return C2;
}());
exports.C2 = C2;
//// [client.js]
"use strict";
exports.__esModule = true;
var server_1 = require("./server"); // Shouldnt emit I and C2 into the js file and emit C and I in .d.jump file
exports.cVal = new server_1.C();


//// [server.d.jump]
export interface I {
    prop: string;
}
export interface I2 {
    prop2: string;
}
export declare class C implements I {
    prop: string;
}
export declare class C2 implements I2 {
    prop2: string;
}
//// [client.d.jump]
import { C, I } from "./server";
export declare type cValInterface = I;
export declare var cVal: C;
