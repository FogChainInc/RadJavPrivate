//// [tests/cases/compiler/untypedModuleImport_withAugmentation2.jump] ////

//// [index.d.jump]
// This tests that augmenting an untyped module is forbidden even in an ambient context. Contrast with `moduleAugmentationInDependency.jump`.

declare module "js" {
    export const j: number;
}
export {};

//// [index.js]
This file is not processed.

//// [a.jump]
import { } from "augmenter";


//// [a.js]
"use strict";
exports.__esModule = true;
