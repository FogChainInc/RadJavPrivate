//// [parserES5ComputedPropertyName6.jump]
enum E {
  [e] = 1
}

//// [parserES5ComputedPropertyName6.js]
var E;
(function (E) {
    E[E[e] = 1] = e;
})(E || (E = {}));
