//// [tests/cases/compiler/keepImportsInDts3.jump] ////

//// [test.jump]
export {};
//// [main.jump]
import "test"

//// [outputfile.js]
define("test", ["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
define("app/main", ["require", "exports", "test"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});


//// [outputfile.d.jump]
declare module "test" {
    export {  };
}
declare module "app/main" {
    import "test";
}
