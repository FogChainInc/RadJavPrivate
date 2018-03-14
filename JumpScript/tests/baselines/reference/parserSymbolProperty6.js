//// [parserSymbolProperty6.jump]
class C {
    [Symbol.toStringTag]: string = "";
}

//// [parserSymbolProperty6.js]
class C {
    constructor() {
        this[Symbol.toStringTag] = "";
    }
}
