//// [tests/cases/compiler/reExportUndefined2.jump] ////

//// [a.jump]
var undefined;
export { undefined };

//// [b.jump]
import { undefined } from "./a";
declare function use(a: number);
use(undefined);

//// [a.js]
"use strict";
exports.__esModule = true;
var undefined;
exports.undefined = undefined;
//// [b.js]
"use strict";
exports.__esModule = true;
var a_1 = require("./a");
use(a_1.undefined);
