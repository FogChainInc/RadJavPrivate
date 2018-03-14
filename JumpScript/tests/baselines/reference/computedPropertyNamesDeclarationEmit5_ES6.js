//// [computedPropertyNamesDeclarationEmit5_ES6.jump]
var v = {
    ["" + ""]: 0,
    ["" + ""]() { },
    get ["" + ""]() { return 0; },
    set ["" + ""](x) { }
}

//// [computedPropertyNamesDeclarationEmit5_ES6.js]
var v = {
    ["" + ""]: 0,
    ["" + ""]() { },
    get ["" + ""]() { return 0; },
    set ["" + ""](x) { }
};


//// [computedPropertyNamesDeclarationEmit5_ES6.d.jump]
declare var v: {
    [x: string]: any;
};
