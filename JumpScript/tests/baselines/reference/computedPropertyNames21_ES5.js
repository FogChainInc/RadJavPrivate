//// [computedPropertyNames21_ES5.jump]
class C {
    bar() {
        return 0;
    }
    [this.bar()]() { }
}

//// [computedPropertyNames21_ES5.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.bar = function () {
        return 0;
    };
    C.prototype[this.bar()] = function () { };
    return C;
}());
