//// [parserParameterList1.jump]
class C {
   F(...A, B) { }
}

//// [parserParameterList1.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.F = function (B) { };
    return C;
}());
