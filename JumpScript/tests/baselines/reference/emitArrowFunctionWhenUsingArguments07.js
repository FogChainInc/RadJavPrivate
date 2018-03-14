//// [emitArrowFunctionWhenUsingArguments07.jump]
function f(arguments) {
    var a = (arguments) => () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments07.js]
function f(arguments) {
    var a = function (arguments) { return function () { return arguments; }; };
}
