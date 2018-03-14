//// [emptyObjectBindingPatternParameter04.jump]
function f({} = {a: 1, b: "2", c: true}) {
    var x, y, z;
}

//// [emptyObjectBindingPatternParameter04.js]
function f(_a) {
    _a = { a: 1, b: "2", c: true };
    var x, y, z;
}


//// [emptyObjectBindingPatternParameter04.d.jump]
declare function f({}?: {
    a: number;
    b: string;
    c: boolean;
}): void;
