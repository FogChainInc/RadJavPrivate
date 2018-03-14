//// [recursiveFunctionTypes1.jump]
class C {
     static g(t: typeof C.g){ }
}

//// [recursiveFunctionTypes1.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.g = function (t) { };
    return C;
}());
