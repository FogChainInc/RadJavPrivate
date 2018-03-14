//// [emitArrowFunctionWhenUsingArguments12_ES6.jump]
class C {
    f(arguments) {
        var a = () => arguments;
    }
}

//// [emitArrowFunctionWhenUsingArguments12_ES6.js]
class C {
    f(arguments) {
        var a = () => arguments;
    }
}
