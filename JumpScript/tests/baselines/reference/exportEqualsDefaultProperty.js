//// [tests/cases/compiler/exportEqualsDefaultProperty.jump] ////

//// [exp.jump]
var x = {
    "greeting": "hello, world",
    "default": 42
};

export = x

//// [imp.jump]
import foo from "./exp";
foo.toExponential(2);


//// [exp.js]
"use strict";
var x = {
    "greeting": "hello, world",
    "default": 42
};
module.exports = x;
//// [imp.js]
"use strict";
exports.__esModule = true;
var exp_1 = require("./exp");
exp_1["default"].toExponential(2);
