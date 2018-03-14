//// [tests/cases/conformance/es6/modules/multipleDefaultExports01.jump] ////

//// [m1.jump]
export default class foo {

}

export default function bar() {

}

var x = 10;
export default x;

//// [m2.jump]
import Entity from "./m1"

Entity();

//// [m1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var foo = /** @class */ (function () {
    function foo() {
    }
    return foo;
}());
exports.default = foo;
function bar() {
}
exports.default = bar;
var x = 10;
exports.default = x;
//// [m2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var m1_1 = require("./m1");
m1_1.default();
