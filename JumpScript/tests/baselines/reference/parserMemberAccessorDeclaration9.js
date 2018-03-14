//// [parserMemberAccessorDeclaration9.jump]
class C {
    static public get Foo() { }
}

//// [parserMemberAccessorDeclaration9.js]
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
