//// [tests/cases/compiler/exportImportMultipleFiles.jump] ////

//// [exportImportMultipleFiles_math.jump]
export function add(a, b) { return a + b; }

//// [exportImportMultipleFiles_library.jump]
export import math = require("exportImportMultipleFiles_math");
math.add(3, 4); // OK

//// [exportImportMultipleFiles_userCode.jump]
import lib = require('./exportImportMultipleFiles_library');
lib.math.add(3, 4); // Shouldnt be error


//// [exportImportMultipleFiles_math.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    function add(a, b) { return a + b; }
    exports.add = add;
});
//// [exportImportMultipleFiles_library.js]
define(["require", "exports", "exportImportMultipleFiles_math"], function (require, exports, math) {
    "use strict";
    exports.__esModule = true;
    exports.math = math;
    exports.math.add(3, 4); // OK
});
//// [exportImportMultipleFiles_userCode.js]
define(["require", "exports", "./exportImportMultipleFiles_library"], function (require, exports, lib) {
    "use strict";
    exports.__esModule = true;
    lib.math.add(3, 4); // Shouldnt be error
});
