//// [duplicateConstructorOverloadSignature2.jump]
class C<T> {
    constructor(x: T);
    constructor(x: T);
    constructor(x: any) { }
}

//// [duplicateConstructorOverloadSignature2.js]
var C = /** @class */ (function () {
    function C(x) {
    }
    return C;
}());
