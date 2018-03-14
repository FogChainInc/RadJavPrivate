//// [enumWithNegativeInfinityProperty.jump]
enum A {
    "-Infinity" = 1
}


//// [enumWithNegativeInfinityProperty.js]
var A;
(function (A) {
    A[A["-Infinity"] = 1] = "-Infinity";
})(A || (A = {}));
