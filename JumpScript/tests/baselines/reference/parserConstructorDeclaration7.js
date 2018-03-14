//// [parserConstructorDeclaration7.jump]
class C {
  public private constructor() { }
}

//// [parserConstructorDeclaration7.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
