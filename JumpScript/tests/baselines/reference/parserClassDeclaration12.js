//// [parserClassDeclaration12.jump]
class C {
   constructor();
   constructor(a) { }
}

//// [parserClassDeclaration12.js]
var C = /** @class */ (function () {
    function C(a) {
    }
    return C;
}());
