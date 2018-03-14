//// [iteratorSpreadInArray10.jump]
class SymbolIterator {
    [Symbol.iterator]() {
        return this;
    }
}

var array = [...new SymbolIterator];

//// [iteratorSpreadInArray10.js]
class SymbolIterator {
    [Symbol.iterator]() {
        return this;
    }
}
var array = [...new SymbolIterator];
