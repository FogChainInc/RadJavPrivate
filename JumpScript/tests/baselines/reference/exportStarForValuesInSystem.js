//// [tests/cases/compiler/exportStarForValuesInSystem.jump] ////

//// [file1.jump]
export interface Foo { x }

//// [file2.jump]
export * from "file1"
var x = 1;

//// [file1.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    return {
        setters: [],
        execute: function () {
        }
    };
});
//// [file2.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var x;
    return {
        setters: [],
        execute: function () {
            x = 1;
        }
    };
});
