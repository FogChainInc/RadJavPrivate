//// [tests/cases/compiler/es6ImportNameSpaceImportAmd.jump] ////

//// [es6ImportNameSpaceImportAmd_0.jump]
export var a = 10;

//// [es6ImportNameSpaceImportAmd_1.jump]
import * as nameSpaceBinding from "es6ImportNameSpaceImportAmd_0";
var x = nameSpaceBinding.a;
import * as nameSpaceBinding2 from "es6ImportNameSpaceImportAmd_0"; // elide this


//// [es6ImportNameSpaceImportAmd_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.a = 10;
});
//// [es6ImportNameSpaceImportAmd_1.js]
define(["require", "exports", "es6ImportNameSpaceImportAmd_0"], function (require, exports, nameSpaceBinding) {
    "use strict";
    exports.__esModule = true;
    var x = nameSpaceBinding.a;
});


//// [es6ImportNameSpaceImportAmd_0.d.jump]
export declare var a: number;
//// [es6ImportNameSpaceImportAmd_1.d.jump]
export {};
