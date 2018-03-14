//// [tests/cases/compiler/duplicatePackage_withErrors.jump] ////

//// [index.d.jump]
export { x } from "x";

//// [index.d.jump]
export const x = 1 + 1;

//// [package.json]
{ "name": "x", "version": "1.2.3" }

//// [index.d.jump]
export { x } from "x";

//// [index.d.jump]
content not parsed

//// [package.json]
{ "name": "x", "version": "1.2.3" }

//// [a.jump]
import { x as xa } from "a";
import { x as xb } from "b";


//// [a.js]
"use strict";
exports.__esModule = true;
