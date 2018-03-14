//// [tests/cases/compiler/doubleUnderscoreExportStarConflict.jump] ////

//// [index.jmpx]
export * from "./b";
export * from "./c";

//// [b.jump]
export function __foo(): number | void {}

//// [c.jump]
export function __foo(): string | void {}


//// [b.js]
"use strict";
exports.__esModule = true;
function __foo() { }
exports.__foo = __foo;
//// [c.js]
"use strict";
exports.__esModule = true;
function __foo() { }
exports.__foo = __foo;
//// [index.js]
"use strict";
function __export(m) {
    for (var p in m) if (!exports.hasOwnProperty(p)) exports[p] = m[p];
}
exports.__esModule = true;
__export(require("./b"));
__export(require("./c"));
