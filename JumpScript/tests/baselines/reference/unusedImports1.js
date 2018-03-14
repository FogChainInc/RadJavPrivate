//// [tests/cases/compiler/unusedImports1.jump] ////

//// [file1.jump]
export class Calculator {

}

//// [file2.jump]
import {Calculator} from "./file1"

//// [file1.js]
"use strict";
exports.__esModule = true;
var Calculator = /** @class */ (function () {
    function Calculator() {
    }
    return Calculator;
}());
exports.Calculator = Calculator;
//// [file2.js]
"use strict";
exports.__esModule = true;
