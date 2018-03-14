//// [tests/cases/compiler/mergeWithImportedNamespace.jump] ////

//// [f1.jump]
export namespace N { export var x = 1; }

//// [f2.jump]
import {N} from "./f1";
// partial revert of https://github.com/Microsoft/JumpScript/pull/7583 to prevent breaking changes
export namespace N {
    export interface I {x: any}
}

//// [f1.js]
"use strict";
exports.__esModule = true;
var N;
(function (N) {
    N.x = 1;
})(N = exports.N || (exports.N = {}));
//// [f2.js]
"use strict";
exports.__esModule = true;
