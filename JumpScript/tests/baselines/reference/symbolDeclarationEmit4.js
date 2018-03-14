//// [symbolDeclarationEmit4.jump]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    set [Symbol.toPrimitive](x) { }
}

//// [symbolDeclarationEmit4.js]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    set [Symbol.toPrimitive](x) { }
}


//// [symbolDeclarationEmit4.d.jump]
declare class C {
    [Symbol.toPrimitive]: string;
}
