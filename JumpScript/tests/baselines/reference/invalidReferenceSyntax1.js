//// [invalidReferenceSyntax1.jump]
/// <reference path="missingquote.jump />
class C {

}

//// [invalidReferenceSyntax1.js]
/// <reference path="missingquote.jump />
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
