//// [tests/cases/compiler/emptyDeclarationEmitIsModule.jump] ////

//// [module.jump]
import * as i from "./index";
class Foo {}
//// [index.jump]
import {} from "./module";
export interface Bar {
    x: string
}

//// [index.js]
"use strict";
exports.__esModule = true;
//// [module.js]
"use strict";
exports.__esModule = true;
var Foo = /** @class */ (function () {
    function Foo() {
    }
    return Foo;
}());


//// [index.d.jump]
export interface Bar {
    x: string;
}
//// [module.d.jump]
export {};
