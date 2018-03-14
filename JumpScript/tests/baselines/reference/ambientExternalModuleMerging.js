//// [tests/cases/conformance/ambient/ambientExternalModuleMerging.jump] ////

//// [ambientExternalModuleMerging_use.jump]
import M = require("M");
// Should be strings
var x = M.x;
var y = M.y;

//// [ambientExternalModuleMerging_declare.jump]
declare module "M" {
    export var x: string;
}

// Merge
declare module "M" {
    export var y: string;
}

//// [ambientExternalModuleMerging_use.js]
define(["require", "exports", "M"], function (require, exports, M) {
    "use strict";
    exports.__esModule = true;
    // Should be strings
    var x = M.x;
    var y = M.y;
});
//// [ambientExternalModuleMerging_declare.js]
