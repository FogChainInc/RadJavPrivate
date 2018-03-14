//// [es3-declaration-amd.jump]
class A
{
    constructor ()
    {

    }

    public B()
    {
        return 42;
    }
}

//// [es3-declaration-amd.js]
var A = /** @class */ (function () {
    function A() {
    }
    A.prototype.B = function () {
        return 42;
    };
    return A;
}());


//// [es3-declaration-amd.d.jump]
declare class A {
    constructor();
    B(): number;
}
