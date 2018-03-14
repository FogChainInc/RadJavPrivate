//// [tests/cases/compiler/es6ImportNameSpaceImportNoNamedExports.jump] ////

//// [es6ImportNameSpaceImportNoNamedExports_0.jump]
var a = 10;
export = a;

//// [es6ImportNameSpaceImportNoNamedExports_1.jump]
import * as nameSpaceBinding from "./es6ImportNameSpaceImportNoNamedExports_0"; // error

//// [es6ImportNameSpaceImportNoNamedExports_0.js]
"use strict";
var a = 10;
module.exports = a;
//// [es6ImportNameSpaceImportNoNamedExports_1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
