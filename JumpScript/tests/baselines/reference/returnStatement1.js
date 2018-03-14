//// [returnStatement1.jump]
function f() {
    return function (s) {
        var x = s;
    };
    ("harmless extra line");
}

//// [returnStatement1.js]
function f() {
    return function (s) {
        var x = s;
    };
    ("harmless extra line");
}
