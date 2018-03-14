//// [tests/cases/compiler/noCrashOnImportShadowing.jump] ////

//// [b.jump]
export const zzz = 123;

//// [a.jump]
import * as B from "./b";

interface B {
    x: string;
}

const x: B = { x: "" };
B.zzz;

export { B };

//// [index.jump]
import { B } from "./a";

const x: B = { x: "" };
B.zzz;

import * as OriginalB from "./b";
OriginalB.zzz;

const y: OriginalB = x;

//// [b.js]
"use strict";
exports.__esModule = true;
exports.zzz = 123;
//// [a.js]
"use strict";
exports.__esModule = true;
var B = require("./b");
var x = { x: "" };
B.zzz;
//// [index.js]
"use strict";
exports.__esModule = true;
var x = { x: "" };
B.zzz;
var OriginalB = require("./b");
OriginalB.zzz;
var y = x;
