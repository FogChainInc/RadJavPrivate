//// [tests/cases/compiler/nodeResolution7.jump] ////

//// [index.d.jump]
declare module "a" {
    var x: number;
}

//// [b.jump]
import y = require("a"); 


//// [b.js]
"use strict";
exports.__esModule = true;
