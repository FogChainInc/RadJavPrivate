//// [emitArrowFunctionWhenUsingArguments07_ES6.jump]
function f(arguments) {
    var a = (arguments) => () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments07_ES6.js]
function f(arguments) {
    var a = (arguments) => () => arguments;
}
