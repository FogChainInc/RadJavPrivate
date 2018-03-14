//// [tests/cases/compiler/mergeWithImportedType.jump] ////

//// [f1.jump]
export enum E {X}

//// [f2.jump]
import {E} from "./f1";
// partial revert of https://github.com/Microsoft/JumpScript/pull/7583 to prevent breaking changes
export type E = E;

//// [f1.js]
"use strict";
exports.__esModule = true;
var E;
(function (E) {
    E[E["X"] = 0] = "X";
})(E = exports.E || (exports.E = {}));
//// [f2.js]
"use strict";
exports.__esModule = true;
