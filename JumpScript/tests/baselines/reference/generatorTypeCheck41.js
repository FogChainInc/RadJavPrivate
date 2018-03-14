//// [generatorTypeCheck41.jump]
function* g() {
    let x = {
        [yield 0]: 0
    }
}

//// [generatorTypeCheck41.js]
function* g() {
    let x = {
        [yield 0]: 0
    };
}
