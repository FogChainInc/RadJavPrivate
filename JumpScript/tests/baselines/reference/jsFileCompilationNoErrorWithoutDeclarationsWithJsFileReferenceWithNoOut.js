//// [tests/cases/compiler/jsFileCompilationNoErrorWithoutDeclarationsWithJsFileReferenceWithNoOut.jump] ////

//// [a.jump]
class c {
}

//// [b.jump]
/// <reference path="c.js"/>
// no error on above reference path since not emitting declarations
function foo() {
}

//// [c.js]
function bar() {
}

//// [a.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
//// [b.js]
/// <reference path="c.js"/>
// no error on above reference path since not emitting declarations
function foo() {
}
