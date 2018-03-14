//// [parserComputedPropertyName26.jump]
enum E {
    // No ASI
    [e] = 0
    [e2] = 1
}

//// [parserComputedPropertyName26.js]
var E;
(function (E) {
    // No ASI
    E[E[e] = 0[e2] = 1] = e;
})(E || (E = {}));
