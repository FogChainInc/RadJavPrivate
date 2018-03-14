//// [tests/cases/compiler/moduleResolution_packageJson_scopedPackage.jump] ////

//// [package.json]
{ "types": "types.d.jump" }

//// [types.d.jump]
export const x: number;

//// [a.jump]
import { x } from "@foo/bar";


//// [a.js]
"use strict";
exports.__esModule = true;
