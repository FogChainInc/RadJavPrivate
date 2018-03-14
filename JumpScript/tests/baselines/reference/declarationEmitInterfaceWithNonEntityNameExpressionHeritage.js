//// [declarationEmitInterfaceWithNonEntityNameExpressionHeritage.jump]
class A { }
interface Class extends (typeof A) { }

//// [declarationEmitInterfaceWithNonEntityNameExpressionHeritage.js]
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());


//// [declarationEmitInterfaceWithNonEntityNameExpressionHeritage.d.jump]
declare class A {
}
interface Class {
}
