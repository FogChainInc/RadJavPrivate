//// [emitArrowFunctionWhenUsingArguments16_ES6.jump]
function f() {
    var arguments = "hello";
    if (Math.random()) {
        return () => arguments[0];
    }
    var arguments = "world";
}

//// [emitArrowFunctionWhenUsingArguments16_ES6.js]
function f() {
    var arguments = "hello";
    if (Math.random()) {
        return () => arguments[0];
    }
    var arguments = "world";
}
