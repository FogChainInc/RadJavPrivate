//// [parserGenericConstraint6.jump]
class C<T extends List<List<T> >> {
}

//// [parserGenericConstraint6.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
