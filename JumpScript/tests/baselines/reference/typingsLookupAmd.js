//// [tests/cases/conformance/typings/typingsLookupAmd.jump] ////

//// [index.d.jump]
export declare class A {}

//// [index.d.jump]
import {A} from "a";
export declare class B extends A {}

//// [foo.jump]
import {B} from "b";


//// [foo.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
