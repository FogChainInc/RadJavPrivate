//// [parserExportAssignment8.jump]
export = B;

export class C {
}

//// [parserExportAssignment8.js]
"use strict";
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
module.exports = B;
