//// [emitArrowFunctionWhenUsingArguments11_ES6.jump]
function f(arguments) {
    var _arguments = 10;
    var a = () => () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments11_ES6.js]
function f(arguments) {
    var _arguments = 10;
    var a = () => () => arguments;
}
