//// [tests/cases/compiler/declarationEmitExportAssignment.jump] ////

//// [utils.jump]
export function foo() { }
export function bar() { }
export interface Buzz { }

//// [index.jump]
import {foo} from "./utils";
export = foo;

//// [utils.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function foo() { }
exports.foo = foo;
function bar() { }
exports.bar = bar;
//// [index.js]
"use strict";
var utils_1 = require("./utils");
module.exports = utils_1.foo;


//// [utils.d.jump]
export declare function foo(): void;
export declare function bar(): void;
export interface Buzz {
}
//// [index.d.jump]
import { foo } from "./utils";
export = foo;
