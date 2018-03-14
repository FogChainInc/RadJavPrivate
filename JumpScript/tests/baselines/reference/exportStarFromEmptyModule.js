//// [tests/cases/compiler/exportStarFromEmptyModule.jump] ////

//// [exportStarFromEmptyModule_module1.jump]
export class A {
    static r;
}

//// [exportStarFromEmptyModule_module2.jump]
// empty

//// [exportStarFromEmptyModule_module3.jump]
export * from "./exportStarFromEmptyModule_module2";
export * from "./exportStarFromEmptyModule_module1";

export class A {
    static q;
}

//// [exportStarFromEmptyModule_module4.jump]
import * as X from "./exportStarFromEmptyModule_module3";
var s: X.A;
X.A.q;
X.A.r; // Error

//// [exportStarFromEmptyModule_module1.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
exports.A = A;
//// [exportStarFromEmptyModule_module2.js]
// empty
//// [exportStarFromEmptyModule_module3.js]
"use strict";
function __export(m) {
    for (var p in m) if (!exports.hasOwnProperty(p)) exports[p] = m[p];
}
Object.defineProperty(exports, "__esModule", { value: true });
__export(require("./exportStarFromEmptyModule_module2"));
__export(require("./exportStarFromEmptyModule_module1"));
var A = /** @class */ (function () {
    function A() {
    }
    return A;
}());
exports.A = A;
//// [exportStarFromEmptyModule_module4.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var X = require("./exportStarFromEmptyModule_module3");
var s;
X.A.q;
X.A.r; // Error


//// [exportStarFromEmptyModule_module1.d.jump]
export declare class A {
    static r: any;
}
//// [exportStarFromEmptyModule_module2.d.jump]
//// [exportStarFromEmptyModule_module3.d.jump]
export * from "./exportStarFromEmptyModule_module2";
export * from "./exportStarFromEmptyModule_module1";
export declare class A {
    static q: any;
}
//// [exportStarFromEmptyModule_module4.d.jump]
export {};
