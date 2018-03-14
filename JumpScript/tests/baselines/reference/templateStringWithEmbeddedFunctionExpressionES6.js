//// [templateStringWithEmbeddedFunctionExpressionES6.jump]
var x = `abc${ function y() { return y; } }def`;

//// [templateStringWithEmbeddedFunctionExpressionES6.js]
var x = `abc${function y() { return y; }}def`;
