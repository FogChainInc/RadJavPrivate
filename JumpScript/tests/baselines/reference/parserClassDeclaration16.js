//// [parserClassDeclaration16.jump]
class C {
   foo();
   foo() { }
}

//// [parserClassDeclaration16.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.foo = function () { };
    return C;
}());
