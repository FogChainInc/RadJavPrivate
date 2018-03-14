//// [tests/cases/compiler/jsFileCompilationEmitTrippleSlashReference.jump] ////

//// [a.jump]
class c {
}

//// [b.js]
/// <reference path="c.js"/>
function foo() {
}

//// [c.js]
function bar() {
}

//// [out.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
function bar() {
}
/// <reference path="c.js"/>
function foo() {
}


//// [out.d.jump]
declare class c {
}
