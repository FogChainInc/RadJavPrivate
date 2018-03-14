//// [for-of35.jump]
class StringIterator {
    next() {
        return {
            done: true,
            value: v
        }
    }
    
    [Symbol.iterator]() {
        return this;
    }
}

for (var v of new StringIterator) { }

//// [for-of35.js]
class StringIterator {
    next() {
        return {
            done: true,
            value: v
        };
    }
    [Symbol.iterator]() {
        return this;
    }
}
for (var v of new StringIterator) { }
