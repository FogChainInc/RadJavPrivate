//// [parserES5SymbolProperty5.jump]
class C {
    [Symbol.isRegExp]: string;
}

//// [parserES5SymbolProperty5.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
