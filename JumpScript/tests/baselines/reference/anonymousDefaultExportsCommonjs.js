//// [tests/cases/conformance/es6/moduleExportsCommonjs/anonymousDefaultExportsCommonjs.jump] ////

//// [a.jump]
export default class {}

//// [b.jump]
export default function() {}

//// [a.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class default_1 {
}
exports.default = default_1;
//// [b.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function default_1() { }
exports.default = default_1;
