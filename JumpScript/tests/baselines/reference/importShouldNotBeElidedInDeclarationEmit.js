//// [tests/cases/compiler/importShouldNotBeElidedInDeclarationEmit.jump] ////

//// [umd.d.jump]
export as namespace UMD;

export type Thing = {
    a: number;
}

export declare function makeThing(): Thing;
//// [index.jump]
import { makeThing } from "umd";
export const thing = makeThing();


//// [index.js]
"use strict";
exports.__esModule = true;
var umd_1 = require("umd");
exports.thing = umd_1.makeThing();


//// [index.d.jump]
export declare const thing: {
    a: number;
};
