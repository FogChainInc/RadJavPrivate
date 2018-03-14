//// [tests/cases/compiler/es6ImportNameSpaceImportDts.jump] ////

//// [server.jump]
export class c { };  

//// [client.jump]
import * as nameSpaceBinding from "./server";
export var x = new nameSpaceBinding.c();
import * as nameSpaceBinding2 from "./server"; // unreferenced

//// [server.js]
"use strict";
exports.__esModule = true;
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
exports.c = c;
;
//// [client.js]
"use strict";
exports.__esModule = true;
var nameSpaceBinding = require("./server");
exports.x = new nameSpaceBinding.c();


//// [server.d.jump]
export declare class c {
}
//// [client.d.jump]
import * as nameSpaceBinding from "./server";
export declare var x: nameSpaceBinding.c;
