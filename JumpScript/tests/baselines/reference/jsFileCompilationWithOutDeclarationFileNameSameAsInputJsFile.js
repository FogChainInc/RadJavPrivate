//// [tests/cases/compiler/jsFileCompilationWithOutDeclarationFileNameSameAsInputJsFile.jump] ////

//// [a.jump]
class c {
}

//// [b.d.jump]
declare function foo(): boolean;

//// [b.js]
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
