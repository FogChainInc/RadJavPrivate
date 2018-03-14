//// [parserES3Accessors2.jump]
class C {
    set Foo(a) { }
}

//// [parserES3Accessors2.js]
var C = /** @class */ (function () {
    function C() {
    }
    Object.defineProperty(C.prototype, "Foo", {
        set: function (a) { },
        enumerable: true,
        configurable: true
    });
    return C;
}());
