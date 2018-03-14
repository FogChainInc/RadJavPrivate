//// [tests/cases/compiler/exportStarForValues2.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export * from "file1"
var x = 1;

//// [file3.jump]
export * from "file2"
var x = 1;

//// [file1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [file2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
//// [file3.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
