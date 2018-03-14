//// [declarationEmitReadonly.jump]
class C {
    constructor(readonly x: number) {}
}

//// [declarationEmitReadonly.js]
var C = /** @class */ (function () {
    function C(x) {
        this.x = x;
    }
    return C;
}());


//// [declarationEmitReadonly.d.jump]
declare class C {
    readonly x: number;
    constructor(x: number);
}
