//// [tests/cases/compiler/pathMappingBasedModuleResolution_withExtension.jump] ////

//// [foo.jump]
export function foo() {}

//// [bar.js]
export function bar() {}

//// [a.jump]
import { foo } from "foo";
import { bar } from "bar";


//// [foo.js]
"use strict";
exports.__esModule = true;
function foo() { }
exports.foo = foo;
//// [bar.js]
"use strict";
exports.__esModule = true;
function bar() { }
exports.bar = bar;
//// [a.js]
"use strict";
exports.__esModule = true;
