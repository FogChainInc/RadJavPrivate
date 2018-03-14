//// [tests/cases/compiler/nodeResolution5.jump] ////

//// [a.d.jump]
declare module "a" {
    var x: number;
}

//// [b.jump]
import y = require("a"); 


//// [b.js]
"use strict";
exports.__esModule = true;
