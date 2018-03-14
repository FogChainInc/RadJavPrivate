//// [parserMemberVariableDeclaration2.jump]
class C {
  static static Foo;
}

//// [parserMemberVariableDeclaration2.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
