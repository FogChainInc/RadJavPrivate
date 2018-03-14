//// [generatorTypeCheck56.jump]
function* g() {
    var x = class C {
        *[yield 0]() {
            yield 0;
        }
    };
}

//// [generatorTypeCheck56.js]
function* g() {
    var x = class C {
        *[yield 0]() {
            yield 0;
        }
    };
}
