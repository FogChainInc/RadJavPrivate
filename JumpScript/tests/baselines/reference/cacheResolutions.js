//// [tests/cases/compiler/cacheResolutions.jump] ////

//// [app.jump]
export let x = 1;

//// [lib1.jump]
export let x = 1;

//// [lib2.jump]
export let x = 1;

//// [app.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x = 1;
});
//// [lib1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x = 1;
});
//// [lib2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x = 1;
});
