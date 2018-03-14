//// [tests/cases/compiler/import_unneeded-require-when-referenecing-aliased-type-throug-array.jump] ////

//// [b.jump]
declare module "ITest" {
    interface Name {
        name: string;
    }
    export = Name;
}

//// [a.jump]
/// <reference path="b.jump" />
import ITest = require('ITest');
var testData: ITest[];
var p = testData[0].name;
 

//// [b.js]
//// [a.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var testData;
    var p = testData[0].name;
});
