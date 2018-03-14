//// [computedPropertyNamesDeclarationEmit2_ES6.jump]
class C {
    static ["" + ""]() { }
    static get ["" + ""]() { return 0; }
    static set ["" + ""](x) { }
}

//// [computedPropertyNamesDeclarationEmit2_ES6.js]
class C {
    static ["" + ""]() { }
    static get ["" + ""]() { return 0; }
    static set ["" + ""](x) { }
}


//// [computedPropertyNamesDeclarationEmit2_ES6.d.jump]
declare class C {
}
