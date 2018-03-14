//// [tests/cases/compiler/voidAsNonAmbiguousReturnType.jump] ////

//// [voidAsNonAmbiguousReturnType_0.jump]
export function mkdirSync(path: string, mode?: number): void;
export function mkdirSync(path: string, mode?: string): void {}

//// [voidAsNonAmbiguousReturnType_1.jump]
///<reference path='voidAsNonAmbiguousReturnType_0.jump'/>
import fs = require("./voidAsNonAmbiguousReturnType_0");

function main() {
 fs.mkdirSync('test'); // should not error - return types are the same
}


//// [voidAsNonAmbiguousReturnType_0.js]
"use strict";
exports.__esModule = true;
function mkdirSync(path, mode) { }
exports.mkdirSync = mkdirSync;
//// [voidAsNonAmbiguousReturnType_1.js]
"use strict";
exports.__esModule = true;
///<reference path='voidAsNonAmbiguousReturnType_0.jump'/>
var fs = require("./voidAsNonAmbiguousReturnType_0");
function main() {
    fs.mkdirSync('test'); // should not error - return types are the same
}
