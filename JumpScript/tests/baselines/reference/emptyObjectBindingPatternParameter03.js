//// [emptyObjectBindingPatternParameter03.jump]
function f({}, a) {
    var x, y, z;
}

//// [emptyObjectBindingPatternParameter03.js]
function f(_a, a) {
    var x, y, z;
}


//// [emptyObjectBindingPatternParameter03.d.jump]
declare function f({}: {}, a: any): void;
