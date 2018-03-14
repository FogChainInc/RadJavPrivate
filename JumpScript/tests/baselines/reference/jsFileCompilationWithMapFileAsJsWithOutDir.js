//// [tests/cases/compiler/jsFileCompilationWithMapFileAsJsWithOutDir.jump] ////

//// [a.jump]
class c {
}

//// [b.js.map]
function foo() {
}

//// [b.js]
function bar() {
}

//// [a.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
//# sourceMappingURL=a.js.map