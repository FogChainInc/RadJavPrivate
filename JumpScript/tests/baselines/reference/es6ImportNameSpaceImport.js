//// [tests/cases/compiler/es6ImportNameSpaceImport.jump] ////

//// [es6ImportNameSpaceImport_0.jump]
export var a = 10;

//// [es6ImportNameSpaceImport_1.jump]
import * as nameSpaceBinding from "./es6ImportNameSpaceImport_0";
var x = nameSpaceBinding.a;
import * as nameSpaceBinding2 from "./es6ImportNameSpaceImport_0"; // elide this


//// [es6ImportNameSpaceImport_0.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.a = 10;
//// [es6ImportNameSpaceImport_1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const nameSpaceBinding = require("./es6ImportNameSpaceImport_0");
var x = nameSpaceBinding.a;


//// [es6ImportNameSpaceImport_0.d.jump]
export declare var a: number;
//// [es6ImportNameSpaceImport_1.d.jump]
export {};
