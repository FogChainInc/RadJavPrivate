//// [tests/cases/compiler/es6ImportDefaultBindingFollowedWithNamespaceBindingWithExport.jump] ////

//// [server.jump]
export var a = 10;

//// [client.jump]
export import defaultBinding, * as nameSpaceBinding  from "./server";
export var x: number = nameSpaceBinding.a;

//// [server.js]
"use strict";
exports.__esModule = true;
exports.a = 10;
//// [client.js]
"use strict";
exports.__esModule = true;
var nameSpaceBinding = require("./server");
exports.x = nameSpaceBinding.a;


//// [server.d.jump]
export declare var a: number;
//// [client.d.jump]
export declare var x: number;
