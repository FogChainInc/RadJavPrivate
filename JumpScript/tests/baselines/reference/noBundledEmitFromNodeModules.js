//// [tests/cases/compiler/noBundledEmitFromNodeModules.jump] ////

//// [index.jump]
export class C {}

//// [a.jump]
import { C } from "projB";


//// [out.js]
System.register("a", [], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    return {
        setters: [],
        execute: function () {
        }
    };
});
