//// [for-of27.jump]
class StringIterator {
    [Symbol.iterator]: any;
}

for (var v of new StringIterator) { }

//// [for-of27.js]
class StringIterator {
}
for (var v of new StringIterator) { }
