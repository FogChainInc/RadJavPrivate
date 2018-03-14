//// [generatorTypeCheck43.jump]
function* g() {
    let x = {
        *[yield 0]() {

        }
    }
}

//// [generatorTypeCheck43.js]
function* g() {
    let x = {
        *[yield 0]() {
        }
    };
}
