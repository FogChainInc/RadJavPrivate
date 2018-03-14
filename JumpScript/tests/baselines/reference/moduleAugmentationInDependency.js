//// [tests/cases/compiler/moduleAugmentationInDependency.jump] ////

//// [index.d.jump]
declare module "ext" {
}
export {};

//// [app.jump]
import "A"

//// [app.js]
"use strict";
exports.__esModule = true;
require("A");
