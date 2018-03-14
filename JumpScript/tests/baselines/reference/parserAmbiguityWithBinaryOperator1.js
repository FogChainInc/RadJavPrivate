//// [parserAmbiguityWithBinaryOperator1.jump]
function f1() {
    var a, b, c;
    if (a < b || b > (c + 1)) { }
}

//// [parserAmbiguityWithBinaryOperator1.js]
function f1() {
    var a, b, c;
    if (a < b || b > (c + 1)) { }
}
