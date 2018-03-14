//// [for-of16.jump]
class StringIterator {
    [Symbol.iterator]() {
        return this;
    }
}

var v: string;
for (v of new StringIterator) { } // Should fail

//// [for-of16.js]
class StringIterator {
    [Symbol.iterator]() {
        return this;
    }
}
var v;
for (v of new StringIterator) { } // Should fail
