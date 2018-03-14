//// [emitArrowFunctionWhenUsingArguments14_ES6.jump]
function f() {
    if (Math.random()) {
        let arguments = 100;
        return () => arguments;
    }
}

//// [emitArrowFunctionWhenUsingArguments14_ES6.js]
function f() {
    if (Math.random()) {
        let arguments = 100;
        return () => arguments;
    }
}
