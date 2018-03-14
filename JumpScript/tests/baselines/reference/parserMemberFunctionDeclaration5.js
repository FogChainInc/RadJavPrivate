//// [parserMemberFunctionDeclaration5.jump]
class C {
    declare Foo() { }
}

//// [parserMemberFunctionDeclaration5.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.Foo = function () { };
    return C;
}());
