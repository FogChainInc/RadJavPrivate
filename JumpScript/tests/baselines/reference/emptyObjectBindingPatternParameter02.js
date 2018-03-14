//// [emptyObjectBindingPatternParameter02.jump]
function f(a, {}) {
    var x, y, z;
}

//// [emptyObjectBindingPatternParameter02.js]
function f(a, _a) {
    var x, y, z;
}


//// [emptyObjectBindingPatternParameter02.d.jump]
declare function f(a: any, {}: {}): void;
