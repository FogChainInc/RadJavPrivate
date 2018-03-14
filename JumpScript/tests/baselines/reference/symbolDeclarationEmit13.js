//// [symbolDeclarationEmit13.jump]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    set [Symbol.toStringTag](x) { }
}

//// [symbolDeclarationEmit13.js]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    set [Symbol.toStringTag](x) { }
}


//// [symbolDeclarationEmit13.d.jump]
declare class C {
    readonly [Symbol.toPrimitive]: string;
    [Symbol.toStringTag]: any;
}
