//// [emitArrowFunctionWhenUsingArguments13.jump]
function f() {
    var _arguments = 10;
    var a = (arguments) => () => _arguments;
}

//// [emitArrowFunctionWhenUsingArguments13.js]
function f() {
    var _arguments = 10;
    var a = function (arguments) { return function () { return _arguments; }; };
}
