//// [computedPropertyNamesDeclarationEmit1_ES6.jump]
class C {
    ["" + ""]() { }
    get ["" + ""]() { return 0; }
    set ["" + ""](x) { }
}

//// [computedPropertyNamesDeclarationEmit1_ES6.js]
class C {
    ["" + ""]() { }
    get ["" + ""]() { return 0; }
    set ["" + ""](x) { }
}


//// [computedPropertyNamesDeclarationEmit1_ES6.d.jump]
declare class C {
}
