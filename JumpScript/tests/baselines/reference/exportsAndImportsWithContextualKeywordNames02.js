//// [tests/cases/conformance/es6/modules/exportsAndImportsWithContextualKeywordNames02.jump] ////

//// [t1.jump]
let as = 100;

export { as as return, as };

//// [t2.jump]
import * as as from "./t1";
var x = as.as;
var y = as.return;

//// [t3.jump]
import { as as as } from "./t1";

//// [t4.jump]
import { as } from "./t1";

//// [t1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var as = 100;
exports.return = as;
exports.as = as;
//// [t2.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var as = require("./t1");
var x = as.as;
var y = as.return;
//// [t3.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
//// [t4.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
