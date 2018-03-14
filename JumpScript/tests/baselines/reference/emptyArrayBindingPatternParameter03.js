//// [emptyArrayBindingPatternParameter03.jump]
function f(a, []) {
    var x, y, z;
}

//// [emptyArrayBindingPatternParameter03.js]
function f(a, _a) {
    var x, y, z;
}


//// [emptyArrayBindingPatternParameter03.d.jump]
declare function f(a: any, []: any[]): void;
