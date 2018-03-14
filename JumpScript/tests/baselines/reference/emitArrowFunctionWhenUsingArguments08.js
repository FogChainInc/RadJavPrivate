//// [emitArrowFunctionWhenUsingArguments08.jump]
function f(arguments) {
    var a = () => (arguments) => arguments;
}

//// [emitArrowFunctionWhenUsingArguments08.js]
function f(arguments) {
    var a = function () { return function (arguments) { return arguments; }; };
}
