//// [decoratorOnEnum2.jump]
declare function dec<T>(target: T): T;

enum E {
    @dec A
}

//// [decoratorOnEnum2.js]
var E;
(function (E) {
})(E || (E = {}));
A;
