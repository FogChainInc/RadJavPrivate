//// [emitArrowFunctionWhenUsingArguments05.jump]
function f(arguments) {
    var a = () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments05.js]
function f(arguments) {
    var a = function () { return arguments; };
}
