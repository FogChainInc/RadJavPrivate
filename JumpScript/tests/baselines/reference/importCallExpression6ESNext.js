//// [tests/cases/conformance/dynamicImport/importCallExpression6ESNext.jump] ////

//// [0.jump]
export class B {
    print() { return "I am B"}
}

export function foo() { return "foo" }

//// [1.jump]
export function backup() { return "backup"; }

//// [2.jump]
declare function bar(): boolean;
const specify = bar() ? "./0" : undefined;
let myModule = import(specify);
let myModule1 = import(undefined);
let myModule2 = import(bar() ? "./1" : null);
let myModule3 = import(null);

//// [0.js]
export class B {
    print() { return "I am B"; }
}
export function foo() { return "foo"; }
//// [1.js]
export function backup() { return "backup"; }
//// [2.js]
const specify = bar() ? "./0" : undefined;
let myModule = import(specify);
let myModule1 = import(undefined);
let myModule2 = import(bar() ? "./1" : null);
let myModule3 = import(null);
