//// [emitArrowFunctionWhenUsingArguments02.jump]
var a = () => arguments;

//// [emitArrowFunctionWhenUsingArguments02.js]
var a = function () { return arguments; };
