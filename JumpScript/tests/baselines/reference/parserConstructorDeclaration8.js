//// [parserConstructorDeclaration8.jump]
class C {
  // Not a constructor
  public constructor;
}

//// [parserConstructorDeclaration8.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
