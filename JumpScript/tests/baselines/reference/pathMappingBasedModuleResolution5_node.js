//// [tests/cases/compiler/pathMappingBasedModuleResolution5_node.jump] ////

//// [file1.jump]
import {x} from "folder2/file1"
import {y} from "folder3/file2"
import {z} from "components/file3"
import {z1} from "file4"

declare function use(a: any): void;

use(x.toExponential());
use(y.toExponential());
use(z.toExponential());
use(z1.toExponential());

//// [file1.jump]
export var x = 1;

//// [file2.jump]
export var y = 1;

//// [index.d.jump]
export var z: number;

//// [file4.jump]
export var z1 = 1;

//// [file1.js]
"use strict";
exports.__esModule = true;
exports.x = 1;
//// [file2.js]
"use strict";
exports.__esModule = true;
exports.y = 1;
//// [file4.js]
"use strict";
exports.__esModule = true;
exports.z1 = 1;
//// [file1.js]
"use strict";
exports.__esModule = true;
var file1_1 = require("folder2/file1");
var file2_1 = require("folder3/file2");
var file3_1 = require("components/file3");
var file4_1 = require("file4");
use(file1_1.x.toExponential());
use(file2_1.y.toExponential());
use(file3_1.z.toExponential());
use(file4_1.z1.toExponential());
