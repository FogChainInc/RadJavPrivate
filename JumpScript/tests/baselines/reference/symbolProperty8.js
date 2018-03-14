//// [symbolProperty8.jump]
interface I {
    [Symbol.unscopables]: number;
    [Symbol.toPrimitive]();
}

//// [symbolProperty8.js]
