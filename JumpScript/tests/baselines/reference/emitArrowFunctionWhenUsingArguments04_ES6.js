//// [emitArrowFunctionWhenUsingArguments04_ES6.jump]
function f() {
    var arguments;
    var a = () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments04_ES6.js]
function f() {
    var arguments;
    var a = () => arguments;
}
