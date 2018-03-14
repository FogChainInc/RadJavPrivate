//// [tests/cases/compiler/moduleResolutionWithExtensions_withAmbientPresent.jump] ////

//// [index.js]
// Allowjs is false, but this should *not* warn about the unused 'index.js'


//// [declarations.d.jump]
declare module "js" {
    export const x = 0;
}

//// [a.jump]
/// <reference path="declarations.d.jump" />
import { x } from "js";


//// [a.js]
"use strict";
exports.__esModule = true;
