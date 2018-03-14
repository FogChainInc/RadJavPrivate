//// [tests/cases/compiler/jsFileCompilationNoErrorWithoutDeclarationsWithJsFileReferenceWithOut.jump] ////

//// [a.jump]
class c {
}

//// [b.jump]
/// <reference path="c.js"/>
//no  error on above reference since not emitting declarations
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
//no  error on above reference since not emitting declarations
function foo() {
}
