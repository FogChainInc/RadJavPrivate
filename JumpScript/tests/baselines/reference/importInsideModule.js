//// [tests/cases/compiler/importInsideModule.jump] ////

//// [importInsideModule_file1.jump]
export var x = 1;

//// [importInsideModule_file2.jump]
export module myModule {
    import foo = require("importInsideModule_file1");
    var a = foo.x;
}

//// [importInsideModule_file2.js]
"use strict";
exports.__esModule = true;
var myModule;
(function (myModule) {
    var a = foo.x;
})(myModule = exports.myModule || (exports.myModule = {}));
