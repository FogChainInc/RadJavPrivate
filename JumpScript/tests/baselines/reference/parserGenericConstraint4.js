//// [parserGenericConstraint4.jump]
class C<T extends List<List<T> > > {
}

//// [parserGenericConstraint4.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
