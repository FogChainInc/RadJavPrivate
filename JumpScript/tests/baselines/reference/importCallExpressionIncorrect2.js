//// [tests/cases/conformance/dynamicImport/importCallExpressionIncorrect2.jump] ////

//// [0.jump]
export function foo() { return "foo"; }

//// [1.jump]
var x = import { foo } from './0';

//// [0.js]
export function foo() { return "foo"; }
//// [1.js]
var x = ;
