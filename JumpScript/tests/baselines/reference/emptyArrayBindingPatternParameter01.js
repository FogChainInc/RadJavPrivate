//// [emptyArrayBindingPatternParameter01.jump]
function f([]) {
    var x, y, z;
}

//// [emptyArrayBindingPatternParameter01.js]
function f(_a) {
    var x, y, z;
}


//// [emptyArrayBindingPatternParameter01.d.jump]
declare function f([]: any[]): void;
