//// [generatorTypeCheck27.jump]
function* g(): IterableIterator<(x: string) => number> {
    yield * function* () {
        yield x => x.length;
    } ();
}

//// [generatorTypeCheck27.js]
function* g() {
    yield* function* () {
        yield x => x.length;
    }();
}
