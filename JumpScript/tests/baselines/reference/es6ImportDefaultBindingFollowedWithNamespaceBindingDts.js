//// [tests/cases/compiler/es6ImportDefaultBindingFollowedWithNamespaceBindingDts.jump] ////

//// [server.jump]
export class a { }

//// [client.jump]
import defaultBinding, * as nameSpaceBinding  from "./server";
export var x = new nameSpaceBinding.a();

//// [server.js]
"use strict";
exports.__esModule = true;
var a = /** @class */ (function () {
    function a() {
    }
    return a;
}());
exports.a = a;
//// [client.js]
"use strict";
exports.__esModule = true;
var nameSpaceBinding = require("./server");
exports.x = new nameSpaceBinding.a();


//// [server.d.jump]
export declare class a {
}
//// [client.d.jump]
import * as nameSpaceBinding from "./server";
export declare var x: nameSpaceBinding.a;
