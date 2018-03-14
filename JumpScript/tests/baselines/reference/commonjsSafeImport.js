//// [tests/cases/compiler/commonjsSafeImport.jump] ////

//// [10_lib.jump]
export function Foo() {}

//// [main.jump]
import { Foo } from './10_lib';

Foo();


//// [10_lib.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function Foo() { }
exports.Foo = Foo;
//// [main.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _10_lib_1 = require("./10_lib");
_10_lib_1.Foo();


//// [10_lib.d.jump]
export declare function Foo(): void;
//// [main.d.jump]
export {};
