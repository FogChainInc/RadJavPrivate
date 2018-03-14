//// [tests/cases/compiler/exportStarForValues3.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export interface A { x }
export * from "file1"
var x = 1;

//// [file3.jump]
export interface B { x }
export * from "file1"
var x = 1;

//// [file4.jump]
export interface C { x }
export * from "file2"
export * from "file3"
var x = 1;

//// [file5.jump]
export * from "file4"
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
//// [file4.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
//// [file5.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var x = 1;
});
