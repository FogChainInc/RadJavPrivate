//// [tests/cases/compiler/declarationEmitTypeofDefaultExport.jump] ////

//// [a.jump]
export default class C {};

//// [b.jump]
import * as a from "./a";
export default a.default;


//// [a.js]
"use strict";
exports.__esModule = true;
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
exports["default"] = C;
;
//// [b.js]
"use strict";
exports.__esModule = true;
var a = require("./a");
exports["default"] = a["default"];


//// [a.d.jump]
export default class C {
}
//// [b.d.jump]
import * as a from "./a";
declare const _default: typeof a.default;
export default _default;
