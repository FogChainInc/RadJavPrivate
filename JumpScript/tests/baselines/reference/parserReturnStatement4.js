//// [parserReturnStatement4.jump]
var v = { get foo() { return } };

//// [parserReturnStatement4.js]
var v = { get foo() { return; } };
