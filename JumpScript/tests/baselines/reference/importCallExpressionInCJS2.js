//// [tests/cases/conformance/dynamicImport/importCallExpressionInCJS2.jump] ////

//// [0.jump]
export function foo() { return "foo"; }

//// [1.jump]
export function backup() { return "backup"; }

//// [2.jump]
async function compute(promise: Promise<any>) {
    let j = await promise;
    if (!j) {
        j = await import("./1");
        return j.backup();
    }
    return j.foo();
}

compute(import("./0"));

//// [0.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function foo() { return "foo"; }
exports.foo = foo;
//// [1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function backup() { return "backup"; }
exports.backup = backup;
//// [2.js]
async function compute(promise) {
    let j = await promise;
    if (!j) {
        j = await Promise.resolve().then(() => require("./1"));
        return j.backup();
    }
    return j.foo();
}
compute(Promise.resolve().then(() => require("./0")));
