//// [functionExpressionReturningItself.jump]
var x = function somefn() { return somefn; };

//// [functionExpressionReturningItself.js]
var x = function somefn() { return somefn; };


//// [functionExpressionReturningItself.d.jump]
declare var x: () => any;
