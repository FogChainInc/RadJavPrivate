//// [symbolDeclarationEmit1.jump]
class C {
    [Symbol.toPrimitive]: number;
}

//// [symbolDeclarationEmit1.js]
class C {
}


//// [symbolDeclarationEmit1.d.jump]
declare class C {
    [Symbol.toPrimitive]: number;
}
