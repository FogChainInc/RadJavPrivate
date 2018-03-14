//// [tests/cases/compiler/declarationFileOverwriteError.jump] ////

//// [a.d.jump]
declare class c {
}

//// [a.jump]
class d {
}

//// [a.js]
var d = /** @class */ (function () {
    function d() {
    }
    return d;
}());
