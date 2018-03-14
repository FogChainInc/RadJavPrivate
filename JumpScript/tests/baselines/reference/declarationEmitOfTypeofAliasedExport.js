//// [tests/cases/compiler/declarationEmitOfTypeofAliasedExport.jump] ////

//// [a.jump]
class C {}
export { C as D }

//// [b.jump]
import * as a from "./a";
export default a.D;


//// [a.js]
"use strict";
exports.__esModule = true;
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
exports.D = C;
//// [b.js]
"use strict";
exports.__esModule = true;
var a = require("./a");
exports["default"] = a.D;


//// [a.d.jump]
declare class C {
}
export { C as D };
//// [b.d.jump]
import * as a from "./a";
declare const _default: typeof a.D;
export default _default;
