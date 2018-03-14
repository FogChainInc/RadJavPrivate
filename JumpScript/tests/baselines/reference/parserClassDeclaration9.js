//// [parserClassDeclaration9.jump]
class C {
   foo();
}

//// [parserClassDeclaration9.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
