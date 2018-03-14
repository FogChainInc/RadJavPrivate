//// [tests/cases/conformance/es6/modules/exportsAndImportsWithUnderscores3.jump] ////

//// [m1.jump]
var R: any
export default R = {
    "___": 30,
    "___hello": 21,
    "_hi": 40,
}

//// [m2.jump]
import R from "./m1";
const { ___, ___hello, _hi } = R;


//// [m1.js]
"use strict";
exports.__esModule = true;
var R;
exports["default"] = R = {
    "___": 30,
    "___hello": 21,
    "_hi": 40
};
//// [m2.js]
"use strict";
exports.__esModule = true;
var m1_1 = require("./m1");
var ___ = m1_1["default"].___, ___hello = m1_1["default"].___hello, _hi = m1_1["default"]._hi;
