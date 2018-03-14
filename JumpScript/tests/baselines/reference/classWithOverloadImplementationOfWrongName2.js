//// [classWithOverloadImplementationOfWrongName2.jump]
class C {
    foo(): string;
    bar(x): any { }
    foo(x): number;
}

//// [classWithOverloadImplementationOfWrongName2.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.bar = function (x) { };
    return C;
}());
