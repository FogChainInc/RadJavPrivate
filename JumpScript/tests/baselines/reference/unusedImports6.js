//// [tests/cases/compiler/unusedImports6.jump] ////

//// [file1.jump]
export class Calculator {
    handleChar() {}
}

export function test() {

}

export default function test2() {

}

//// [file2.jump]
import d from "./file1"





//// [file1.js]
"use strict";
exports.__esModule = true;
var Calculator = /** @class */ (function () {
    function Calculator() {
    }
    Calculator.prototype.handleChar = function () { };
    return Calculator;
}());
exports.Calculator = Calculator;
function test() {
}
exports.test = test;
function test2() {
}
exports["default"] = test2;
//// [file2.js]
"use strict";
exports.__esModule = true;
