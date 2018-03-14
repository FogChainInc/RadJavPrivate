//// [tests/cases/compiler/jsFileCompilationWithoutOut.jump] ////

//// [a.jump]
class c {
}

//// [b.js]
function foo() {
}


//// [a.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
