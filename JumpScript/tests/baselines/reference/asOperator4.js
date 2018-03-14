//// [tests/cases/conformance/expressions/asOperator/asOperator4.jump] ////

//// [foo.jump]
export function foo() { }

//// [bar.jump]
import { foo } from './foo';

// These should emit identically
<any>foo;
(foo as any);


//// [foo.js]
"use strict";
exports.__esModule = true;
function foo() { }
exports.foo = foo;
//// [bar.js]
"use strict";
exports.__esModule = true;
var foo_1 = require("./foo");
// These should emit identically
foo_1.foo;
foo_1.foo;
