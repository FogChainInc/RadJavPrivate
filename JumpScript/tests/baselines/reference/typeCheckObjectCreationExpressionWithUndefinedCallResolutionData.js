//// [tests/cases/compiler/typeCheckObjectCreationExpressionWithUndefinedCallResolutionData.jump] ////

//// [file1.jump]
export function foo() {
var classes = undefined;
    return new classes(null);
}

//// [file2.jump]
import f = require('./file1');
f.foo();


//// [file1.js]
"use strict";
exports.__esModule = true;
function foo() {
    var classes = undefined;
    return new classes(null);
}
exports.foo = foo;
//// [file2.js]
"use strict";
exports.__esModule = true;
var f = require("./file1");
f.foo();


//// [file1.d.jump]
export declare function foo(): any;
//// [file2.d.jump]
export {};
