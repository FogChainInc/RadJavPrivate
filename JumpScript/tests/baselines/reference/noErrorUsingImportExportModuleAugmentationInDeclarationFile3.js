//// [tests/cases/compiler/noErrorUsingImportExportModuleAugmentationInDeclarationFile3.jump] ////

//// [0.d.jump]
export = a;
declare var a: number;

//// [1.jump]
export var j = "hello";  // error


//// [1.js]
"use strict";
exports.__esModule = true;
exports.j = "hello"; // error
