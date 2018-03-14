//// [tests/cases/compiler/systemExportAssignment.jump] ////

//// [a.d.jump]
declare var a: number;
export = a;  // OK, in ambient context

//// [b.jump]
import * as a from "a";


//// [b.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    return {
        setters: [],
        execute: function () {
        }
    };
});
