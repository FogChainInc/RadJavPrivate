//// [tests/cases/conformance/dynamicImport/importCallExpression2ESNext.jump] ////

//// [0.jump]
export class B {
    print() { return "I am B"}
}

//// [2.jump]
function foo(x: Promise<any>) {
    x.then(value => {
        let b = new value.B();
        b.print();
    })
}

foo(import("./0"));

//// [0.js]
export class B {
    print() { return "I am B"; }
}
//// [2.js]
function foo(x) {
    x.then(value => {
        let b = new value.B();
        b.print();
    });
}
foo(import("./0"));
