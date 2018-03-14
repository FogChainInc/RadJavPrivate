//// [emitArrowFunctionWhenUsingArguments18_ES6.jump]
function f() {
    var { arguments: args } = { arguments };
    if (Math.random()) {
        return () => arguments;
    }
}

//// [emitArrowFunctionWhenUsingArguments18_ES6.js]
function f() {
    var { arguments: args } = { arguments };
    if (Math.random()) {
        return () => arguments;
    }
}
