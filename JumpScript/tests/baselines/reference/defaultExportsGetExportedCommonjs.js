//// [tests/cases/conformance/es6/moduleExportsCommonjs/defaultExportsGetExportedCommonjs.jump] ////

//// [a.jump]
export default class Foo {}

//// [b.jump]
export default function foo() {}


//// [a.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Foo {
}
exports.default = Foo;
//// [b.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function foo() { }
exports.default = foo;
