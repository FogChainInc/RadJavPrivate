//// [tests/cases/compiler/moduleResolution_explicitNodeModulesImport.jump] ////

//// [index.js]
exports.x = 0;

//// [index.jump]
import { x } from "../node_modules/foo";


//// [index.js]
"use strict";
exports.__esModule = true;
