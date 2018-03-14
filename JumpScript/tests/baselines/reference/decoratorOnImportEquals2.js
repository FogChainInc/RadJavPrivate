//// [tests/cases/conformance/decorators/invalid/decoratorOnImportEquals2.jump] ////

//// [decoratorOnImportEquals2_0.jump]
export var X;

//// [decoratorOnImportEquals2_1.jump]
@dec
import lib = require('./decoratorOnImportEquals2_0');

declare function dec<T>(target: T): T;

//// [decoratorOnImportEquals2_0.js]
"use strict";
exports.__esModule = true;
//// [decoratorOnImportEquals2_1.js]
"use strict";
exports.__esModule = true;
