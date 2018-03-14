//// [tests/cases/compiler/exportSpecifierForAGlobal.jump] ////

//// [a.d.jump]
declare class X { }

//// [b.jump]
export {X};
export function f() {
    var x: X;
    return x;
} 


//// [b.js]
"use strict";
exports.__esModule = true;
function f() {
    var x;
    return x;
}
exports.f = f;
