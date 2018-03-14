//// [generatorES6_2.jump]
class C {
    public * foo() {
        yield 1
    }
}

//// [generatorES6_2.js]
class C {
    *foo() {
        yield 1;
    }
}
