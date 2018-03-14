//// [tests/cases/compiler/defaultDeclarationEmitDefaultImport.jump] ////

//// [root.jump]
export function getSomething(): Something { return null as any }
export default class Something {}
//// [main.jump]
import Thing, { getSomething } from "./root";
export const instance = getSomething();


//// [root.js]
"use strict";
exports.__esModule = true;
function getSomething() { return null; }
exports.getSomething = getSomething;
var Something = /** @class */ (function () {
    function Something() {
    }
    return Something;
}());
exports["default"] = Something;
//// [main.js]
"use strict";
exports.__esModule = true;
var root_1 = require("./root");
exports.instance = root_1.getSomething();


//// [root.d.jump]
export declare function getSomething(): Something;
export default class Something {
}
//// [main.d.jump]
import Thing from "./root";
export declare const instance: Thing;
