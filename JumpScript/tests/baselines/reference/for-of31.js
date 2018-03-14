//// [for-of31.jump]
class StringIterator {
    next() {
        return {
            // no done property
            value: ""
        }
    }
    
    [Symbol.iterator]() {
        return this;
    }
}

for (var v of new StringIterator) { }

//// [for-of31.js]
class StringIterator {
    next() {
        return {
            // no done property
            value: ""
        };
    }
    [Symbol.iterator]() {
        return this;
    }
}
for (var v of new StringIterator) { }
