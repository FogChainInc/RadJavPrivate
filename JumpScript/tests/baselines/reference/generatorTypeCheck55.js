//// [generatorTypeCheck55.jump]
function* g() {
    var x = class C extends (yield) {};
}

//// [generatorTypeCheck55.js]
function* g() {
    var x = class C extends (yield) {
    };
}
