//// [requiredInitializedParameter4.jump]
class C1 {
    method(a = 0, b) { }
}

//// [requiredInitializedParameter4.js]
var C1 = /** @class */ (function () {
    function C1() {
    }
    C1.prototype.method = function (a, b) {
        if (a === void 0) { a = 0; }
    };
    return C1;
}());


//// [requiredInitializedParameter4.d.jump]
declare class C1 {
    method(a: number, b: any): void;
}
