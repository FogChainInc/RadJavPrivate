//// [tests/cases/compiler/declarationEmitInferredTypeAlias5.jump] ////

//// [0.jump]
export type Data = string | boolean;
let obj: Data = true;

//// [1.jump]
import * as Z from "./0"
//let v2: Z.Data;
let v = "str" || true;
export { v }

//// [0.js]
"use strict";
exports.__esModule = true;
var obj = true;
//// [1.js]
"use strict";
exports.__esModule = true;
//let v2: Z.Data;
var v = "str" || true;
exports.v = v;


//// [0.d.jump]
export declare type Data = string | boolean;
//// [1.d.jump]
import * as Z from "./0";
declare let v: Z.Data;
export { v };
