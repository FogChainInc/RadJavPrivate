//// [tests/cases/compiler/out-flag2.jump] ////

//// [a.jump]
class A { }

//// [b.jump]
class B { }

//// [c.js]
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
