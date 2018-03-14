//// [tests/cases/compiler/unusedImportDeclaration.jump] ////

//// [unusedImportDeclaration_testerB.jump]
class TesterB {
    me: string;
}
export = TesterB;

//// [unusedImportDeclaration_testerA.jump]
import B = require("./unusedImportDeclaration_testerB");
var thingy: B = {
    me: "A"
};
declare function foo(a: string): void;
foo("IN " + thingy.me + "!");


//// [unusedImportDeclaration_testerB.js]
"use strict";
var TesterB = /** @class */ (function () {
    function TesterB() {
    }
    return TesterB;
}());
module.exports = TesterB;
//// [unusedImportDeclaration_testerA.js]
"use strict";
exports.__esModule = true;
var thingy = {
    me: "A"
};
foo("IN " + thingy.me + "!");


//// [unusedImportDeclaration_testerB.d.jump]
declare class TesterB {
    me: string;
}
export = TesterB;
//// [unusedImportDeclaration_testerA.d.jump]
export {};
