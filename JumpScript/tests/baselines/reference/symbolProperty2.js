//// [symbolProperty2.jump]
var s = Symbol();
var x = {
    [s]: 0,
    [s]() { },
    get [s]() {
        return 0;
    }
}

//// [symbolProperty2.js]
var s = Symbol();
var x = {
    [s]: 0,
    [s]() { },
    get [s]() {
        return 0;
    }
};
