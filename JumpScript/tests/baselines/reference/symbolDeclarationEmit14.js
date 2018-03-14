//// [symbolDeclarationEmit14.jump]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    get [Symbol.toStringTag]() { return ""; }
}

//// [symbolDeclarationEmit14.js]
class C {
    get [Symbol.toPrimitive]() { return ""; }
    get [Symbol.toStringTag]() { return ""; }
}


//// [symbolDeclarationEmit14.d.jump]
declare class C {
    readonly [Symbol.toPrimitive]: string;
    readonly [Symbol.toStringTag]: string;
}
