//// [tests/cases/compiler/narrowedImports_assumeInitialized.jump] ////

//// [a.d.jump]
declare namespace a {
    export const x: number;
}
export = a;

//// [b.jump]
import a = require("./a");
a.x;


//// [b.js]
"use strict";
exports.__esModule = true;
var a = require("./a");
a.x;
