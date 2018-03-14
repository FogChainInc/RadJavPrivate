//// [emitClassDeclarationOverloadInES6.jump]
class C {
    constructor(y: any)
    constructor(x: number) {
    }
}

class D {
    constructor(y: any)
    constructor(x: number, z="hello") {}
}

//// [emitClassDeclarationOverloadInES6.js]
class C {
    constructor(x) {
    }
}
class D {
    constructor(x, z = "hello") { }
}
