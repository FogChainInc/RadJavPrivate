//// [tests/cases/compiler/exportAssignmentFunction.jump] ////

//// [exportAssignmentFunction_A.jump]
function foo() { return 0; }

export = foo;

//// [exportAssignmentFunction_B.jump]
import fooFunc = require("exportAssignmentFunction_A");

var n: number = fooFunc();

//// [exportAssignmentFunction_A.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    function foo() { return 0; }
    return foo;
});
//// [exportAssignmentFunction_B.js]
define(["require", "exports", "exportAssignmentFunction_A"], function (require, exports, fooFunc) {
    "use strict";
    exports.__esModule = true;
    var n = fooFunc();
});
