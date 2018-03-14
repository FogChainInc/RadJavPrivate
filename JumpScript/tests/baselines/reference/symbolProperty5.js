//// [symbolProperty5.jump]
var x = {
    [Symbol.iterator]: 0,
    [Symbol.toPrimitive]() { },
    get [Symbol.toStringTag]() {
        return 0;
    }
}

//// [symbolProperty5.js]
var x = {
    [Symbol.iterator]: 0,
    [Symbol.toPrimitive]() { },
    get [Symbol.toStringTag]() {
        return 0;
    }
};
