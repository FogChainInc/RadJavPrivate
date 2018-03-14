//// [tests/cases/compiler/declarationEmitAliasExportStar.jump] ////

//// [thingB.jump]
export interface ThingB { }
//// [things.jump]
export * from "./thingB";
//// [index.jump]
import * as things from "./things";
export const thing2 = (param: things.ThingB) => null;


//// [thingB.js]
"use strict";
exports.__esModule = true;
//// [things.js]
"use strict";
exports.__esModule = true;
//// [index.js]
"use strict";
exports.__esModule = true;
exports.thing2 = function (param) { return null; };


//// [thingB.d.jump]
export interface ThingB {
}
//// [things.d.jump]
export * from "./thingB";
//// [index.d.jump]
import * as things from "./things";
export declare const thing2: (param: things.ThingB) => any;
