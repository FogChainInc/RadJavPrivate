//// [tests/cases/compiler/pathMappingBasedModuleResolution_withExtensionInName.jump] ////

//// [index.d.jump]
export const x: number;

//// [index.d.jump]
export const y: number;

//// [a.jump]
import { x } from "zone.js";
import { y } from "zone.jmpx";


//// [a.js]
"use strict";
exports.__esModule = true;
