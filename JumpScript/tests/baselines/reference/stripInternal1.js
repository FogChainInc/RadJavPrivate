//// [stripInternal1.jump]
class C {
  foo(): void { }
  // @internal
  bar(): void { }
}

//// [stripInternal1.js]
var C = /** @class */ (function () {
    function C() {
    }
    C.prototype.foo = function () { };
    // @internal
    C.prototype.bar = function () { };
    return C;
}());


//// [stripInternal1.d.jump]
declare class C {
    foo(): void;
}
