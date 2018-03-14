//// [tests/cases/compiler/out-flag3.jump] ////

//// [a.jump]
// --out and --outFile error

class A { }

//// [b.jump]
class B { }

//// [c.js]
// --out and --outFile error
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
var B = /** @class */ (function () {
    function B() {
    }
    return B;
}());
//# sourceMappingURL=c.js.map

//// [c.d.jump]
declare class A {
}
declare class B {
}
