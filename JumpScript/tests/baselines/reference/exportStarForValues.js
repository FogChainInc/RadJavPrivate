//// [tests/cases/compiler/exportStarForValues.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export * from "file1"
var x;

//// [file1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [file2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x;
});
