//// [staticMustPrecedePublic.jump]
class Outer {
    static public intI: number;
    static private stringF: string;
}


//// [staticMustPrecedePublic.js]
var Outer = /** @class */ (function () {
    function Outer() {
    }
    return Outer;
}());
