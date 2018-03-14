//// [tests/cases/conformance/externalModules/invalidSyntaxNamespaceImportWithCommonjs.jump] ////

//// [0.jump]
export class C { }

//// [1.jump]
import * from Zero from "./0"

//// [0.js]
"use strict";
exports.__esModule = true;
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
exports.C = C;
//// [1.js]
"use strict";
exports.__esModule = true;
var from = require();
from;
"./0";
