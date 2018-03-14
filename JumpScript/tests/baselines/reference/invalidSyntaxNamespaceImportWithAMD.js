//// [tests/cases/conformance/externalModules/invalidSyntaxNamespaceImportWithAMD.jump] ////

//// [0.jump]
export class C { }

//// [1.jump]
import * from Zero from "./0"

//// [0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var C = /** @class */ (function () {
        function C() {
        }
        return C;
    }());
    exports.C = C;
});
//// [1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    from;
    "./0";
});
