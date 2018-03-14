//// [symbolDeclarationEmit2.jump]
class C {
    [Symbol.toPrimitive] = "";
}

//// [symbolDeclarationEmit2.js]
class C {
    constructor() {
        this[Symbol.toPrimitive] = "";
    }
}


//// [symbolDeclarationEmit2.d.jump]
declare class C {
    [Symbol.toPrimitive]: string;
}
