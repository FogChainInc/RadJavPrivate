//// [tests/cases/compiler/jmpconfigMapOptionsAreCaseInsensitive.jump] ////

//// [other.jump]
export default 42;

//// [index.jump]
import Answer from "./other.js";
const x = 10 + Answer;
export {
    x
};

//// [other.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports["default"] = 42;
});
//// [index.js]
define(["require", "exports", "./other.js"], function (require, exports, other_js_1) {
    "use strict";
    exports.__esModule = true;
    var x = 10 + other_js_1["default"];
    exports.x = x;
});
