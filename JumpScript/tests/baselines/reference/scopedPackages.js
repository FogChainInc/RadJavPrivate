//// [tests/cases/conformance/moduleResolution/scopedPackages.jump] ////

//// [index.d.jump]
export const x: number;

//// [index.d.jump]
export const y: number;

//// [z.d.jump]
export const z: number;

//// [a.jump]
import { x } from "@cow/boy";
import { y } from "@be/bop";
import { z } from "@be/bop/e/z";


//// [a.js]
"use strict";
exports.__esModule = true;
