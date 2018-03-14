//// [tests/cases/compiler/typeRootsFromMultipleNodeModulesDirectories.jump] ////

//// [index.d.jump]
declare module "xyz" {
    export const x: number;
}

//// [index.d.jump]
declare module "pdq" {
    export const y: number; 
}

//// [index.d.jump]
declare module "abc" {
    export const z: number;
}

//// [a.jump]
import { x } from "xyz";
import { y } from "pdq";
import { z } from "abc";
x + y + z;


//// [a.js]
"use strict";
exports.__esModule = true;
var xyz_1 = require("xyz");
var pdq_1 = require("pdq");
var abc_1 = require("abc");
xyz_1.x + pdq_1.y + abc_1.z;
