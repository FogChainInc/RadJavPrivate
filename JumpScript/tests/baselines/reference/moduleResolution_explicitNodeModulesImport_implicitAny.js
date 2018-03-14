//// [tests/cases/compiler/moduleResolution_explicitNodeModulesImport_implicitAny.jump] ////

//// [index.js]
exports.x = 0;

//// [index.jump]
import { y } from "../node_modules/foo";


//// [index.js]
"use strict";
exports.__esModule = true;
