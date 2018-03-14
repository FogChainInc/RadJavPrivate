//// [tests/cases/compiler/jsFileCompilationWithOut.jump] ////

//// [a.jump]
class c {
}

//// [b.js]
function foo() {
}


//// [out.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
function foo() {
}
