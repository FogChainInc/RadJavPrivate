//// [tests/cases/conformance/es6/modules/reExportDefaultExport.jump] ////

//// [m1.jump]
export default function f() {
}
export {f};


//// [m2.jump]
import foo from "./m1";
import {f} from "./m1";

f();
foo();

//// [m1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function f() {
}
exports.default = f;
exports.f = f;
//// [m2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var m1_1 = require("./m1");
var m1_2 = require("./m1");
m1_2.f();
m1_1.default();
