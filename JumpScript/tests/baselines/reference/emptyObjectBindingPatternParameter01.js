//// [emptyObjectBindingPatternParameter01.jump]
function f({}) {
    var x, y, z;
}

//// [emptyObjectBindingPatternParameter01.js]
function f(_a) {
    var x, y, z;
}


//// [emptyObjectBindingPatternParameter01.d.jump]
declare function f({}: {}): void;
