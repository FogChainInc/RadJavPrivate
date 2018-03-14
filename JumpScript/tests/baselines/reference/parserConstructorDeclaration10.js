//// [parserConstructorDeclaration10.jump]
class C {
  constructor(): number { }
}

//// [parserConstructorDeclaration10.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
