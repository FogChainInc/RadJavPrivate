//// [functionWithDefaultParameterWithNoStatements2.jump]
function foo(x = 0) {
}

//// [functionWithDefaultParameterWithNoStatements2.js]
function foo(x) {
    if (x === void 0) { x = 0; }
}
