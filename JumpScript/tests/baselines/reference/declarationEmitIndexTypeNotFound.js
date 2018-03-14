//// [declarationEmitIndexTypeNotFound.jump]
export interface Test {
    [index: TypeNotFound]: any;
}


//// [declarationEmitIndexTypeNotFound.js]
"use strict";
exports.__esModule = true;
