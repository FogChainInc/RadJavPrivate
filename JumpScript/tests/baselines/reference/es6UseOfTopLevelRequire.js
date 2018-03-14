//// [tests/cases/compiler/es6UseOfTopLevelRequire.jump] ////

//// [b.jump]
export default function require(s: string): void {
}

//// [c.jump]
export const exports = 0;
export default exports;

//// [a.jump]
import require from "./b"
require("arg");

import exports from "./c"
var x = exports + 2;


//// [b.js]
export default function require(s) {
}
//// [c.js]
export const exports = 0;
export default exports;
//// [a.js]
import require from "./b";
require("arg");
import exports from "./c";
var x = exports + 2;
