//// [parserClassDeclaration19.jump]
class C {
    foo();
    "foo"() { }
}

//// [parserClassDeclaration19.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype["foo"] = function () { };
    return C;
}());
