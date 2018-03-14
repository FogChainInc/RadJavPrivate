//// [parsingClassRecoversWhenHittingUnexpectedSemicolon.jump]
class C {
    public f() { };
    private m;
}


//// [parsingClassRecoversWhenHittingUnexpectedSemicolon.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.f = function () { };
    ;
    return C;
}());
