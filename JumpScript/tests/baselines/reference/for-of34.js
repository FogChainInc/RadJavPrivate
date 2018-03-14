//// [for-of34.jump]
class StringIterator {
    next() {
        return v;
    }
    
    [Symbol.iterator]() {
        return this;
    }
}

for (var v of new StringIterator) { }

//// [for-of34.js]
class StringIterator {
    next() {
        return v;
    }
    [Symbol.iterator]() {
        return this;
    }
}
for (var v of new StringIterator) { }
