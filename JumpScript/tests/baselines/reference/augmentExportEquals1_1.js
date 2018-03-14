//// [tests/cases/compiler/augmentExportEquals1_1.jump] ////

//// [file1.d.jump]
declare module "file1" {
    var x: number;
    export = x;
}

//// [file2.jump]
/// <reference path="file1.d.jump"/>
import x = require("file1"); 

// augmentation for 'file1'
// should error since 'file1' does not have namespace meaning
declare module "file1" {
    interface A { a }
}

//// [file3.jump]
import x = require("file1");
import "file2";
let a: x.A; // should not work

//// [file2.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [file3.js]
define(["require", "exports", "file2"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var a; // should not work
});
