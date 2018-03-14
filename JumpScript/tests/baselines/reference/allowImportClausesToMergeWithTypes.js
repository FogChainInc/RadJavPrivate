//// [tests/cases/compiler/allowImportClausesToMergeWithTypes.jump] ////

//// [b.jump]
export const zzz = 123;
export default zzz;

//// [a.jump]
export default interface zzz {
    x: string;
}

import zzz from "./b";

const x: zzz = { x: "" };
zzz;

export { zzz as default };

//// [index.jump]
import zzz from "./a";

const x: zzz = { x: "" };
zzz;

import originalZZZ from "./b";
originalZZZ;

const y: originalZZZ = x;

//// [b.js]
"use strict";
exports.__esModule = true;
exports.zzz = 123;
exports["default"] = exports.zzz;
//// [a.js]
"use strict";
exports.__esModule = true;
var b_1 = require("./b");
exports["default"] = b_1["default"];
var x = { x: "" };
b_1["default"];
//// [index.js]
"use strict";
exports.__esModule = true;
var x = { x: "" };
zzz;
var b_1 = require("./b");
b_1["default"];
var y = x;
