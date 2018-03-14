//// [tests/cases/compiler/functionAndImportNameConflict.jump] ////

//// [f1.jump]
export function f() {
}

//// [f2.jump]
import {f} from './f1';
export function f() {
}

//// [f1.js]
"use strict";
exports.__esModule = true;
function f() {
}
exports.f = f;
//// [f2.js]
"use strict";
exports.__esModule = true;
function f() {
}
exports.f = f;
