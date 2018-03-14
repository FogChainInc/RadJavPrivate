//// [parserIndexMemberDeclaration3.jump]
class C {
   [a: string]: number;
   public v: number
}

//// [parserIndexMemberDeclaration3.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
