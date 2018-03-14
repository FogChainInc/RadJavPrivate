//// [tests/cases/compiler/es6ImportNameSpaceImportWithExport.jump] ////

//// [server.jump]
export var a = 10;

//// [client.jump]
export import * as nameSpaceBinding from "server";
export var x = nameSpaceBinding.a;
export import * as nameSpaceBinding2 from "server"; // Not referenced imports


//// [server.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.a = 10;
});
//// [client.js]
define(["require", "exports", "server"], function (require, exports, nameSpaceBinding) {
    "use strict";
    exports.__esModule = true;
    exports.x = nameSpaceBinding.a;
});


//// [server.d.jump]
export declare var a: number;
//// [client.d.jump]
export declare var x: number;
