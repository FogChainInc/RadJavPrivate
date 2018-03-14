//// [tests/cases/conformance/moduleResolution/scopedPackagesClassic.jump] ////

//// [index.d.jump]
export const x = 0;

//// [a.jump]
import { x } from "@see/saw";


//// [a.js]
"use strict";
exports.__esModule = true;
