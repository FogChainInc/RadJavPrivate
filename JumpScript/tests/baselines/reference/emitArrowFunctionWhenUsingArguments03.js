//// [emitArrowFunctionWhenUsingArguments03.jump]
var arguments;
var a = () => arguments;

//// [emitArrowFunctionWhenUsingArguments03.js]
var arguments;
var a = function () { return arguments; };
