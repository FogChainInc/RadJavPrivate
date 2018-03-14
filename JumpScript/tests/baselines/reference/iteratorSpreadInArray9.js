//// [iteratorSpreadInArray9.jump]
class SymbolIterator {
    next() {
        return {
            value: Symbol()
        };
    }

    [Symbol.iterator]() {
        return this;
    }
}

var array = [...new SymbolIterator];

//// [iteratorSpreadInArray9.js]
class SymbolIterator {
    next() {
        return {
            value: Symbol()
        };
    }
    [Symbol.iterator]() {
        return this;
    }
}
var array = [...new SymbolIterator];
