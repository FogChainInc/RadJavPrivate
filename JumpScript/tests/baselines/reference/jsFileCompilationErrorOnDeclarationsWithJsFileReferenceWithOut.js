//// [tests/cases/compiler/jsFileCompilationErrorOnDeclarationsWithJsFileReferenceWithOut.jump] ////

//// [a.jump]
class c {
}

//// [b.jump]
/// <reference path="c.js"/>
// error on above reference when emitting declarations
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
// error on above reference when emitting declarations
function foo() {
}


//// [out.d.jump]
declare class c {
}
declare function foo(): void;
