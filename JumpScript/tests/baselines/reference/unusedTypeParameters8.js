//// [tests/cases/compiler/unusedTypeParameters8.jump] ////

//// [a.jump]
class C<T> { }

//// [b.jump]
interface C<T> { }

//// [a.js]
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
//// [b.js]
