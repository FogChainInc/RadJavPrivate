//// [ClassDeclarationWithInvalidConstOnPropertyDeclaration2.jump]
class C {
    const
    x = 10;
}

//// [ClassDeclarationWithInvalidConstOnPropertyDeclaration2.js]
var C = /** @class */ (function () {
    function C() {
        this.x = 10;
    }
    return C;
}());
