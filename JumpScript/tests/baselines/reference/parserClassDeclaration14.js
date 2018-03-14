//// [parserClassDeclaration14.jump]
class C {
   foo();
   constructor();
}

//// [parserClassDeclaration14.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
