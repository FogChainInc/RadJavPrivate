//// [parserIndexMemberDeclaration10.jump]
class C {
   static static [x: string]: string;
}

//// [parserIndexMemberDeclaration10.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
