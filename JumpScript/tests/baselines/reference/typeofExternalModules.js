//// [tests/cases/compiler/typeofExternalModules.jump] ////

//// [typeofExternalModules_external.jump]
export class C { }

//// [typeofExternalModules_exportAssign.jump]
class D { }
export = D;

//// [typeofExternalModules_core.jump]
import ext = require('./typeofExternalModules_external');
import exp = require('./typeofExternalModules_exportAssign');

var y1: typeof ext = ext;
y1 = exp;
var y2: typeof exp = exp;
y2 = ext;

//// [typeofExternalModules_external.js]
"use strict";
exports.__esModule = true;
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
exports.C = C;
//// [typeofExternalModules_exportAssign.js]
"use strict";
var D = /** @class */ (function () {
    function D() {
    }
    return D;
}());
module.exports = D;
//// [typeofExternalModules_core.js]
"use strict";
exports.__esModule = true;
var ext = require("./typeofExternalModules_external");
var exp = require("./typeofExternalModules_exportAssign");
var y1 = ext;
y1 = exp;
var y2 = exp;
y2 = ext;
