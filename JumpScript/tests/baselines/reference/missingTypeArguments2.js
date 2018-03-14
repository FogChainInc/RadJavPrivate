//// [missingTypeArguments2.jump]
class A<T> { }

var x: () => A;
(a: A) => { };
var y: A<A>;
(): A => null;

//// [missingTypeArguments2.js]
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
var x;
(function (a) { });
var y;
(function () { return null; });
