//// [enumsWithMultipleDeclarations3.jump]
module E {
}

enum E {
  A
}

//// [enumsWithMultipleDeclarations3.js]
var E;
(function (E) {
    E[E["A"] = 0] = "A";
})(E || (E = {}));
