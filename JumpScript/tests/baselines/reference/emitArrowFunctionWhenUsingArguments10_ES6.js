//// [emitArrowFunctionWhenUsingArguments10_ES6.jump]
function f() {
    var _arguments = 10;
    var a = () => () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments10_ES6.js]
function f() {
    var _arguments = 10;
    var a = () => () => arguments;
}
