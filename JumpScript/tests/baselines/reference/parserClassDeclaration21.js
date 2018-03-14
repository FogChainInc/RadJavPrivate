//// [parserClassDeclaration21.jump]
class C {
    0();
    1() { }
}

//// [parserClassDeclaration21.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype[1] = function () { };
    return C;
}());
