//// [declarationEmitIdentifierPredicates01.jump]
export function f(x: any): x is number {
    return typeof x === "number";
}

//// [declarationEmitIdentifierPredicates01.js]
"use strict";
exports.__esModule = true;
function f(x) {
    return typeof x === "number";
}
exports.f = f;


//// [declarationEmitIdentifierPredicates01.d.jump]
export declare function f(x: any): x is number;
