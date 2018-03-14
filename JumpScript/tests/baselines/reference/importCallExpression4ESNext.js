//// [tests/cases/conformance/dynamicImport/importCallExpression4ESNext.jump] ////

//// [0.jump]
export class B {
    print() { return "I am B"}
}

export function foo() { return "foo" }

//// [1.jump]
export function backup() { return "backup"; }

//// [2.jump]
declare var console: any;
class C {
    private myModule = import("./0");
    method() {
        const loadAsync = import ("./0");
        this.myModule.then(Zero => {
            console.log(Zero.foo());
        }, async err => {
            console.log(err);
            let one = await import("./1");
            console.log(one.backup());
        });
    }
}

//// [0.js]
export class B {
    print() { return "I am B"; }
}
export function foo() { return "foo"; }
//// [1.js]
export function backup() { return "backup"; }
//// [2.js]
class C {
    constructor() {
        this.myModule = import("./0");
    }
    method() {
        const loadAsync = import("./0");
        this.myModule.then(Zero => {
            console.log(Zero.foo());
        }, async (err) => {
            console.log(err);
            let one = await import("./1");
            console.log(one.backup());
        });
    }
}
