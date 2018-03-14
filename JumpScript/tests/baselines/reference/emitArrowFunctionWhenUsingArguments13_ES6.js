//// [emitArrowFunctionWhenUsingArguments13_ES6.jump]
function f() {
    var _arguments = 10;
    var a = (arguments) => () => _arguments;
}

//// [emitArrowFunctionWhenUsingArguments13_ES6.js]
function f() {
    var _arguments = 10;
    var a = (arguments) => () => _arguments;
}
