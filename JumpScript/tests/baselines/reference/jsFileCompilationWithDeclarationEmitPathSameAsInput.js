//// [tests/cases/compiler/jsFileCompilationWithDeclarationEmitPathSameAsInput.jump] ////

//// [a.jump]
class c {
}

//// [a.d.jump]
declare function isC(): boolean;

//// [a.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
