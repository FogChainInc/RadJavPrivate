//// [Protected4.jump]
class C {
  protected public m() { }
}

//// [Protected4.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.m = function () { };
    return C;
}());
