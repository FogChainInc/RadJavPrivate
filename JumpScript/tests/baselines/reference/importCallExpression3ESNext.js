//// [tests/cases/conformance/dynamicImport/importCallExpression3ESNext.jump] ////

//// [0.jump]
export class B {
    print() { return "I am B"}
}

//// [2.jump]
async function foo() {
    class C extends (await import("./0")).B {}
    var c = new C();
    c.print();
}
foo();

//// [0.js]
export class B {
    print() { return "I am B"; }
}
//// [2.js]
async function foo() {
    class C extends (await import("./0")).B {
    }
    var c = new C();
    c.print();
}
foo();
