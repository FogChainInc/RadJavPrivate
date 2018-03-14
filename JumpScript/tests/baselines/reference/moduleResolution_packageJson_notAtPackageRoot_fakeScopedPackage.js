//// [tests/cases/compiler/moduleResolution_packageJson_notAtPackageRoot_fakeScopedPackage.jump] ////

//// [package.json]
// Copy of `moduleResolution_packageJson_notAtPackageRoot` with `foo/@bar` instead of `foo/bar`. Should behave identically.

{ "types": "types.d.jump" }

//// [package.json]
{}

//// [types.d.jump]
export const x: number;

//// [a.jump]
import { x } from "foo/@bar";


//// [a.js]
"use strict";
exports.__esModule = true;
