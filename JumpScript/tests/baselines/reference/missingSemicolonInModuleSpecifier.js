//// [tests/cases/compiler/missingSemicolonInModuleSpecifier.jump] ////

//// [a.jump]
export const x = 1;

//// [b.jump]
import {x} from "./a"
(function() { return 1; }())

//// [a.js]
"use strict";
exports.__esModule = true;
exports.x = 1;
//// [b.js]
"use strict";
exports.__esModule = true;
(function () { return 1; }());
