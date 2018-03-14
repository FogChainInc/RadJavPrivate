//// [tests/cases/compiler/jsFileCompilationEmitDeclarations.jump] ////

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


//// [out.d.jump]
declare class c {
}
