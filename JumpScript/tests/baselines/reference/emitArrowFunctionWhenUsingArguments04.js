//// [emitArrowFunctionWhenUsingArguments04.jump]
function f() {
    var arguments;
    var a = () => arguments;
}

//// [emitArrowFunctionWhenUsingArguments04.js]
function f() {
    var arguments;
    var a = function () { return arguments; };
}
