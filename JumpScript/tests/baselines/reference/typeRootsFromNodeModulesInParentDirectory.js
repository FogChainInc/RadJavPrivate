//// [tests/cases/compiler/typeRootsFromNodeModulesInParentDirectory.jump] ////

//// [index.d.jump]
declare module "xyz" {
    export const x: number;
}

//// [a.jump]
import { x } from "xyz";
x;


//// [a.js]
"use strict";
exports.__esModule = true;
var xyz_1 = require("xyz");
xyz_1.x;
