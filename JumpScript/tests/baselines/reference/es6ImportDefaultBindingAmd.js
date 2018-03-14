//// [tests/cases/compiler/es6ImportDefaultBindingAmd.jump] ////

//// [es6ImportDefaultBindingAmd_0.jump]
var a = 10;
export default a;

//// [es6ImportDefaultBindingAmd_1.jump]
import defaultBinding from "es6ImportDefaultBindingAmd_0";
var x = defaultBinding;
import defaultBinding2 from "es6ImportDefaultBindingAmd_0"; // elide this import since defaultBinding2 is not used


//// [es6ImportDefaultBindingAmd_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    var a = 10;
    exports.default = a;
});
//// [es6ImportDefaultBindingAmd_1.js]
define(["require", "exports", "es6ImportDefaultBindingAmd_0"], function (require, exports, es6ImportDefaultBindingAmd_0_1) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    var x = es6ImportDefaultBindingAmd_0_1.default;
});


//// [es6ImportDefaultBindingAmd_0.d.jump]
declare var a: number;
export default a;
//// [es6ImportDefaultBindingAmd_1.d.jump]
export {};
