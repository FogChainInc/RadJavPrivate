//// [for-of33.jump]
class StringIterator {
    [Symbol.iterator]() {
        return v;
    }
}

for (var v of new StringIterator) { }

//// [for-of33.js]
class StringIterator {
    [Symbol.iterator]() {
        return v;
    }
}
for (var v of new StringIterator) { }
