//// [staticIndexer.jump]
class C {
    static [s: string]: number;
}

//// [staticIndexer.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
