//// [tests/cases/compiler/pathMappingBasedModuleResolution7_node.jump] ////

//// [file1.jump]
import {x} from "./project/file2";
import {y} from "module3";

declare function use(x: string);
use(x.toFixed());
use(y.toFixed());

//// [file2.jump]
import {a} from "module1";
import {b} from "templates/module2";
import {x as c} from "../file3";
export let x = a + b + c;

//// [index.d.jump]
export let a: number

//// [module2.jump]
export let b: number;

//// [index.d.jump]
export let x: number;

//// [module3.d.jump]
export let y: number;



//// [module2.js]
"use strict";
exports.__esModule = true;
//// [file2.js]
"use strict";
exports.__esModule = true;
var module1_1 = require("module1");
var module2_1 = require("templates/module2");
var file3_1 = require("../file3");
exports.x = module1_1.a + module2_1.b + file3_1.x;
//// [file1.js]
"use strict";
exports.__esModule = true;
var file2_1 = require("./project/file2");
var module3_1 = require("module3");
use(file2_1.x.toFixed());
use(module3_1.y.toFixed());
