//// [tests/cases/conformance/dynamicImport/importCallExpressionDeclarationEmit2.jump] ////

//// [0.jump]
export function foo() { return "foo"; }

//// [1.jump]
var p1 = import("./0");

//// [0.js]
export function foo() { return "foo"; }
//// [1.js]
var p1 = import("./0");


//// [0.d.jump]
export declare function foo(): string;
