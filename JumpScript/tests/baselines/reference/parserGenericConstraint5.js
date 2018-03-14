//// [parserGenericConstraint5.jump]
class C<T extends List<List<T>> > {
}

//// [parserGenericConstraint5.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
