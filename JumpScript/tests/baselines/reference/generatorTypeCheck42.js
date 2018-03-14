//// [generatorTypeCheck42.jump]
function* g() {
    let x = {
        [yield 0]() {

        }
    }
}

//// [generatorTypeCheck42.js]
function* g() {
    let x = {
        [yield 0]() {
        }
    };
}
