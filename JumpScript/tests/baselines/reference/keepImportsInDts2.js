//// [tests/cases/compiler/keepImportsInDts2.jump] ////

//// [test.jump]
export {};
//// [main.jump]
import "./folder/test"

//// [test.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [main.js]
define(["require", "exports", "./folder/test"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});


//// [test.d.jump]
export {  };
//// [main.d.jump]
import "./folder/test";
