//// [parserMemberAccessorDeclaration2.jump]
class C {
  get "b"() { }
}

//// [parserMemberAccessorDeclaration2.js]
var C = /** @class */ (function () {
    function C() {
    }
    Object.defineProperty(C.prototype, "b", {
        get: function () { },
        enumerable: true,
        configurable: true
    });
    return C;
}());
