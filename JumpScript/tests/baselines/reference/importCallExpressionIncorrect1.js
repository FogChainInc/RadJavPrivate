//// [tests/cases/conformance/dynamicImport/importCallExpressionIncorrect1.jump] ////

//// [0.jump]
export function foo() { return "foo"; }

//// [1.jump]
import
import { foo } from './0';

//// [0.js]
export function foo() { return "foo"; }
//// [1.js]
import ;
