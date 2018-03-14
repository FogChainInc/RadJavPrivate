//// [tests/cases/compiler/noErrorUsingImportExportModuleAugmentationInDeclarationFile2.jump] ////

//// [1.jump]
export var j = "hello"; // error

//// [0.d.jump]
export = a;
declare var a: number;

//// [1.js]
"use strict";
exports.__esModule = true;
exports.j = "hello"; // error
