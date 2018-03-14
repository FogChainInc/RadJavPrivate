//// [tests/cases/conformance/es6/modules/multipleDefaultExports02.jump] ////

//// [m1.jump]
export default function foo() {

}

export default function bar() {

}

//// [m2.jump]
import Entity from "./m1"

Entity();

//// [m1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function foo() {
}
exports.default = foo;
function bar() {
}
exports.default = bar;
//// [m2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var m1_1 = require("./m1");
m1_1.default();
