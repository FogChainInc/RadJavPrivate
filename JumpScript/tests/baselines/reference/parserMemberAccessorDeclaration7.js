//// [parserMemberAccessorDeclaration7.jump]
class C {
    public public get Foo() { }
}

//// [parserMemberAccessorDeclaration7.js]
var C = /** @class */ (function () {
    function C() {
    }
    Object.defineProperty(C.prototype, "Foo", {
        get: function () { },
        enumerable: true,
        configurable: true
    });
    return C;
}());
