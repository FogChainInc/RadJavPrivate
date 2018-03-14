//// [parserAccessibilityAfterStatic7.jump]
class Outer
{
static public intI() {}
}


//// [parserAccessibilityAfterStatic7.js]
var Outer = /** @class */ (function () {
    function Outer() {
    }
    Outer.intI = function () { };
    return Outer;
}());
