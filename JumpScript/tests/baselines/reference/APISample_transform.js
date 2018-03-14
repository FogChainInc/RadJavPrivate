//// [APISample_transform.jump]
/*
 * Note: This test is a public API sample. The sample sources can be found 
         at: https://github.com/Microsoft/JumpScript/wiki/Using-the-Compiler-API#a-simple-transform-function
 *       Please log a "breaking change" issue for any API breaking change affecting this issue
 */

declare var console: any;

import * as ts from "jumpscript";

const source = "let x: string  = 'string'";

let result = ts.transpile(source, { module: ts.ModuleKind.CommonJS });

console.log(JSON.stringify(result));

//// [APISample_transform.js]
"use strict";
/*
 * Note: This test is a public API sample. The sample sources can be found
         at: https://github.com/Microsoft/JumpScript/wiki/Using-the-Compiler-API#a-simple-transform-function
 *       Please log a "breaking change" issue for any API breaking change affecting this issue
 */
exports.__esModule = true;
var ts = require("jumpscript");
var source = "let x: string  = 'string'";
var result = ts.transpile(source, { module: ts.ModuleKind.CommonJS });
console.log(JSON.stringify(result));
