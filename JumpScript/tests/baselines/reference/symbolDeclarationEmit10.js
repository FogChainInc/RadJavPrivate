//// [symbolDeclarationEmit10.jump]
var obj = {
    get [Symbol.isConcatSpreadable]() { return '' },
    set [Symbol.isConcatSpreadable](x) { }
}

//// [symbolDeclarationEmit10.js]
var obj = {
    get [Symbol.isConcatSpreadable]() { return ''; },
    set [Symbol.isConcatSpreadable](x) { }
};


//// [symbolDeclarationEmit10.d.jump]
declare var obj: {
    [Symbol.isConcatSpreadable]: string;
};
