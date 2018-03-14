//// [tests/cases/compiler/constEnumExternalModule.jump] ////

//// [m1.jump]
const enum E {
    V = 100
}

export = E
//// [m2.jump]
import A = require('m1')
var v = A.V;

//// [m1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [m2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var v = 100 /* V */;
});
