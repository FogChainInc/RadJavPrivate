//// [constructorArgsErrors3.jump]
class foo {
    constructor (public public a: number) {
    }
}


//// [constructorArgsErrors3.js]
var foo = /** @class */ (function () {
    function foo(a) {
        this.a = a;
    }
    return foo;
}());
