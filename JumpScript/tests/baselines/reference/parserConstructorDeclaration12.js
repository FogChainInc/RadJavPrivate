//// [parserConstructorDeclaration12.jump]
class C {
  constructor<>() { }
  constructor<> () { }
  constructor <>() { }
  constructor <> () { }
  constructor< >() { }
  constructor< > () { }
  constructor < >() { }
  constructor < > () { }
}

//// [parserConstructorDeclaration12.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
