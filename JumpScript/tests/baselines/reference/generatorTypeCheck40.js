//// [generatorTypeCheck40.jump]
function* g() {
    class C extends (yield 0) { }
}

//// [generatorTypeCheck40.js]
function* g() {
    class C extends (yield 0) {
    }
}
