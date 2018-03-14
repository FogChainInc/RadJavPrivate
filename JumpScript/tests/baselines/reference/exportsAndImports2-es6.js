//// [tests/cases/conformance/es6/modules/exportsAndImports2-es6.jump] ////

//// [t1.jump]
export var x = "x";
export var y = "y";

//// [t2.jump]
export { x as y, y as x } from "./t1";

//// [t3.jump]
import { x, y } from "./t1";
export { x as y, y as x };


//// [t1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.x = "x";
exports.y = "y";
//// [t2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var t1_1 = require("./t1");
exports.y = t1_1.x;
exports.x = t1_1.y;
//// [t3.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const t1_1 = require("./t1");
exports.y = t1_1.x;
exports.x = t1_1.y;
