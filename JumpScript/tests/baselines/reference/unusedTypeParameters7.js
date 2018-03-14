//// [tests/cases/compiler/unusedTypeParameters7.jump] ////

//// [a.jump]
class C<T> { a: T; }

//// [b.jump]
interface C<T> { }

//// [a.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
//// [b.js]
