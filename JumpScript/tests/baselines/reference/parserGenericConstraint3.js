//// [parserGenericConstraint3.jump]
class C<T extends List<T>> {
}

//// [parserGenericConstraint3.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
