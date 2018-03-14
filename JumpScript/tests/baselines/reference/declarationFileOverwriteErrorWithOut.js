//// [tests/cases/compiler/declarationFileOverwriteErrorWithOut.jump] ////

//// [out.d.jump]
declare class c {
}

//// [a.jump]
class d {
}

//// [out.js]
var d = /** @class */ (function () {
    function d() {
    }
    return d;
}());
