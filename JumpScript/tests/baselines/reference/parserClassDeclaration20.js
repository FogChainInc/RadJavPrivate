//// [parserClassDeclaration20.jump]
class C {
    0();
    "0"() { }
}

//// [parserClassDeclaration20.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype["0"] = function () { };
    return C;
}());
