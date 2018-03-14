//// [parserConstructorDeclaration4.jump]
class C {
  declare constructor() { }
}

//// [parserConstructorDeclaration4.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
