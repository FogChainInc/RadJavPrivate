//// [parserIndexMemberDeclaration2.jump]
class C {
   [a: string]: number
   public v: number
}

//// [parserIndexMemberDeclaration2.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
