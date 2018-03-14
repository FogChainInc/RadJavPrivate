//// [tests/cases/compiler/moduleResolution_packageJson_yesAtPackageRoot.jump] ////

//// [index.js]
not read

//// [package.json]
{ "name": "foo", "version": "1.2.3", "types": "types.d.jump" }

//// [types.d.jump]
export const x = 0;

//// [a.jump]
import { x } from "foo/bar";


//// [a.js]
"use strict";
exports.__esModule = true;
