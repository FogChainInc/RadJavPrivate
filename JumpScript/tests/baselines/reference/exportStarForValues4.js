//// [tests/cases/compiler/exportStarForValues4.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export interface A { x }
export * from "file1"
export * from "file3"
var x = 1;

//// [file3.jump]
export interface B { x }
export * from "file2"
var x = 1;


//// [file1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [file3.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
//// [file2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
