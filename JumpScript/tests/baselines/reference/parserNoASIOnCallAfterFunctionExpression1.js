//// [parserNoASIOnCallAfterFunctionExpression1.jump]
var x = function () { }
(<any>window).foo;


//// [parserNoASIOnCallAfterFunctionExpression1.js]
var x = function () { }(window).foo;
