//// [parserComputedPropertyName29.jump]
class C {
    // yes ASI
    [e] = id++
    [e2]: number
}

//// [parserComputedPropertyName29.js]
class C {
    constructor() {
        // yes ASI
        this[_a] = id++;
    }
}
_a = e;
var _a;
