//// [generatorTypeCheck7.jump]
interface WeirdIter extends IterableIterator<number> {
    hello: string;
}
function* g1(): WeirdIter { }

//// [generatorTypeCheck7.js]
function* g1() { }
