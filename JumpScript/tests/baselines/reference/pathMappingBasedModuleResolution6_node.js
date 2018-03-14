//// [tests/cases/compiler/pathMappingBasedModuleResolution6_node.jump] ////

//// [file1.jump]
import {x} from "./project/file3";
declare function use(x: string);
use(x.toFixed());

//// [index.d.jump]
export let x: number;

//// [file3.jump]
export {x} from "../file2";

//// [file3.js]
"use strict";
exports.__esModule = true;
var file2_1 = require("../file2");
exports.x = file2_1.x;
//// [file1.js]
"use strict";
exports.__esModule = true;
var file3_1 = require("./project/file3");
use(file3_1.x.toFixed());
