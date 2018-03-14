//// [emptyArrayBindingPatternParameter04.jump]
function f([] = [1,2,3,4]) {
    var x, y, z;
}

//// [emptyArrayBindingPatternParameter04.js]
function f(_a) {
    _a = [1, 2, 3, 4];
    var x, y, z;
}


//// [emptyArrayBindingPatternParameter04.d.jump]
declare function f([]?: number[]): void;
