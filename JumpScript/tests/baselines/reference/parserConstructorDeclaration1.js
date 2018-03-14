//// [parserConstructorDeclaration1.jump]
class C {
 public constructor() { }
}

//// [parserConstructorDeclaration1.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
