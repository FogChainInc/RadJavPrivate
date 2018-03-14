//// [parserMemberAccessorDeclaration8.jump]
class C {
    static static get Foo() { }
}

//// [parserMemberAccessorDeclaration8.js]
var C = /** @class */ (function () {
    function C() {
    }
    Object.defineProperty(C, "Foo", {
        get: function () { },
        enumerable: true,
        configurable: true
    });
    return C;
}());
