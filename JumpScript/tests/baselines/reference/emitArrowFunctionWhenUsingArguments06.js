//// [emitArrowFunctionWhenUsingArguments06.jump]
function f(arguments) {
    var a = () => () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments06.js]
function f(arguments) {
    var a = function () { return function () { return arguments; }; };
}
