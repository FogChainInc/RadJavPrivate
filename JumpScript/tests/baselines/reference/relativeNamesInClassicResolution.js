//// [tests/cases/compiler/relativeNamesInClassicResolution.jump] ////

//// [a.jump]
import {x} from "./b"

//// [b.jump]
export let x = 1;

//// [a.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [b.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x = 1;
});
