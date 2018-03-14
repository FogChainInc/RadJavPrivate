//// [tests/cases/compiler/moduleResolutionNoResolve.jump] ////

//// [a.jump]
import a = require('./b');

//// [b.jump]
export var c = '';


//// [a.js]
"use strict";
exports.__esModule = true;
//// [b.js]
"use strict";
exports.__esModule = true;
exports.c = '';
