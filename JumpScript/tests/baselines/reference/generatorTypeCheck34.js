//// [generatorTypeCheck34.jump]
function* g() {
    yield 0;
    function* g2() {
        return "";
    }
}

//// [generatorTypeCheck34.js]
function* g() {
    yield 0;
    function* g2() {
        return "";
    }
}
