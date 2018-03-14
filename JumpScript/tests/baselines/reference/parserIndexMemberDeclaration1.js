//// [parserIndexMemberDeclaration1.jump]
class C {
   [a: string]: number
}

//// [parserIndexMemberDeclaration1.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
