//// [enumWithParenthesizedInitializer1.jump]
enum E {
 e = -(3
}

//// [enumWithParenthesizedInitializer1.js]
var E;
(function (E) {
    E[E["e"] = -3] = "e";
})(E || (E = {}));
