//// [tests/cases/compiler/keepImportsInDts1.jump] ////

//// [test.d.jump]
export {}; 
//// [main.jump]
import "test"

//// [main.js]
define(["require", "exports", "test"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});


//// [main.d.jump]
import "test";
