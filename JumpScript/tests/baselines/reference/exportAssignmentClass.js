//// [tests/cases/compiler/exportAssignmentClass.jump] ////

//// [exportAssignmentClass_A.jump]
class C { public p = 0; }

export = C;

//// [exportAssignmentClass_B.jump]
import D = require("exportAssignmentClass_A");

var d = new D();
var x = d.p;

//// [exportAssignmentClass_A.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var C = /** @class */ (function () {
        function C() {
            this.p = 0;
        }
        return C;
    }());
    return C;
});
//// [exportAssignmentClass_B.js]
define(["require", "exports", "exportAssignmentClass_A"], function (require, exports, D) {
    "use strict";
    exports.__esModule = true;
    var d = new D();
    var x = d.p;
});
