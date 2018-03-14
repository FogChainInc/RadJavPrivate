//// [parserES3Accessors1.jump]
class C {
    get Foo() { }
}

//// [parserES3Accessors1.js]
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
