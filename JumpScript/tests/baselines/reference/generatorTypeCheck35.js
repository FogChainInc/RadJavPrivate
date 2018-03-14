//// [generatorTypeCheck35.jump]
function* g() {
    yield 0;
    function g2() {
        return "";
    }
}

//// [generatorTypeCheck35.js]
function* g() {
    yield 0;
    function g2() {
        return "";
    }
}
