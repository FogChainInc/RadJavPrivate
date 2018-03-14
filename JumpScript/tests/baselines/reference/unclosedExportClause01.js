//// [tests/cases/compiler/unclosedExportClause01.jump] ////

//// [t1.jump]
export var x = "x";

//// [t2.jump]
export { x, from "./t1"

//// [t3.jump]
export { from "./t1"

//// [t4.jump]
export { x as a from "./t1"

//// [t5.jump]
export { x as a, from "./t1"

//// [t1.js]
"use strict";
exports.__esModule = true;
exports.x = "x";
//// [t2.js]
"use strict";
exports.__esModule = true;
var t1_1 = require("./t1");
exports.x = t1_1.x;
exports.from = t1_1.from;
//// [t3.js]
"use strict";
exports.__esModule = true;
var t1_1 = require("./t1");
exports.from = t1_1.from;
//// [t4.js]
"use strict";
exports.__esModule = true;
var t1_1 = require("./t1");
exports.a = t1_1.x;
exports.from = t1_1.from;
//// [t5.js]
"use strict";
exports.__esModule = true;
var t1_1 = require("./t1");
exports.a = t1_1.x;
exports.from = t1_1.from;
