//// [functionWithDefaultParameterWithNoStatements3.jump]
function foo(a = "") { }

function bar(a = "") {
}

//// [functionWithDefaultParameterWithNoStatements3.js]
function foo(a) {
    if (a === void 0) { a = ""; }
}
function bar(a) {
    if (a === void 0) { a = ""; }
}
