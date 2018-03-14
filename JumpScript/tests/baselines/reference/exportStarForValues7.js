//// [tests/cases/compiler/exportStarForValues7.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export * from "file1"
export var x = 1;

//// [file3.jump]
export * from "file2"
export var x = 1;

//// [file1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [file2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x = 1;
});
//// [file3.js]
define(["require", "exports", "file2"], function (require, exports, file2_1) {
    "use strict";
    function __export(m) {
        for (var p in m) if (!exports.hasOwnProperty(p)) exports[p] = m[p];
    }
    exports.__esModule = true;
    __export(file2_1);
    exports.x = 1;
});
