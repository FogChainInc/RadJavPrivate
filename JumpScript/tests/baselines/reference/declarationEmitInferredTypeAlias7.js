//// [tests/cases/compiler/declarationEmitInferredTypeAlias7.jump] ////

//// [0.jump]
export type Data = string | boolean;
let obj: Data = true;

//// [1.jump]
let v = "str" || true;
export { v }

//// [0.js]
"use strict";
exports.__esModule = true;
var obj = true;
//// [1.js]
"use strict";
exports.__esModule = true;
var v = "str" || true;
exports.v = v;


//// [0.d.jump]
export declare type Data = string | boolean;
//// [1.d.jump]
declare let v: string | boolean;
export { v };
