//// [symbolDeclarationEmit8.jump]
var obj = {
    [Symbol.isConcatSpreadable]: 0
}

//// [symbolDeclarationEmit8.js]
var obj = {
    [Symbol.isConcatSpreadable]: 0
};


//// [symbolDeclarationEmit8.d.jump]
declare var obj: {
    [Symbol.isConcatSpreadable]: number;
};
