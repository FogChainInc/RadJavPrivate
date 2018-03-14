//// [computedPropertyNames23_ES6.jump]
class C {
    bar() {
        return 0;
    }
    [
        { [this.bar()]: 1 }[0]
    ]() { }
}

//// [computedPropertyNames23_ES6.js]
class C {
    bar() {
        return 0;
    }
    [{ [this.bar()]: 1 }[0]]() { }
}
