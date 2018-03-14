//// [symbolDeclarationEmit9.jump]
var obj = {
    [Symbol.isConcatSpreadable]() { }
}

//// [symbolDeclarationEmit9.js]
var obj = {
    [Symbol.isConcatSpreadable]() { }
};


//// [symbolDeclarationEmit9.d.jump]
declare var obj: {
    [Symbol.isConcatSpreadable](): void;
};
