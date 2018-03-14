//// [computedPropertyNames21_ES6.jump]
class C {
    bar() {
        return 0;
    }
    [this.bar()]() { }
}

//// [computedPropertyNames21_ES6.js]
class C {
    bar() {
        return 0;
    }
    [this.bar()]() { }
}
