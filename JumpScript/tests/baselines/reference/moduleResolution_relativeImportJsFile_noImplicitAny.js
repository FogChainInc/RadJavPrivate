//// [tests/cases/compiler/moduleResolution_relativeImportJsFile_noImplicitAny.jump] ////

//// [b.js]
export const x = 0;

//// [a.jump]
import * as b from "./b";


//// [a.js]
"use strict";
exports.__esModule = true;
