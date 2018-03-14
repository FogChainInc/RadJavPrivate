//// [ClassDeclaration11.jump]
class C {
   constructor();
   foo() { }
}

//// [ClassDeclaration11.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.foo = function () { };
    return C;
}());
