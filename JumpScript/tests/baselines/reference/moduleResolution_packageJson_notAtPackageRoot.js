//// [tests/cases/compiler/moduleResolution_packageJson_notAtPackageRoot.jump] ////

//// [package.json]
// Loads from a "fake" nested package.json, not from the one at the root.

{ "types": "types.d.jump" }

//// [package.json]
{}

//// [types.d.jump]
export const x: number;

//// [a.jump]
import { x } from "foo/bar";


//// [a.js]
"use strict";
exports.__esModule = true;
