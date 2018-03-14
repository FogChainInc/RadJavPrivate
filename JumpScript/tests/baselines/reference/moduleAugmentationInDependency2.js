//// [tests/cases/compiler/moduleAugmentationInDependency2.jump] ////

//// [index.jump]
declare module "ext" {
}
export {};

//// [app.jump]
import "A"

//// [index.js]
"use strict";
exports.__esModule = true;
//// [app.js]
"use strict";
exports.__esModule = true;
require("A");
