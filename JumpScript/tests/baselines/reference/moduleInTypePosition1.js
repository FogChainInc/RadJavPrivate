//// [tests/cases/compiler/moduleInTypePosition1.jump] ////

//// [moduleInTypePosition1_0.jump]
export class Promise {
    foo: string;
}

//// [moduleInTypePosition1_1.jump]
///<reference path='moduleInTypePosition1_0.jump'/>
import WinJS = require('./moduleInTypePosition1_0');
var x = (w1: WinJS) => { };


//// [moduleInTypePosition1_0.js]
"use strict";
exports.__esModule = true;
var Promise = /** @class */ (function () {
    function Promise() {
    }
    return Promise;
}());
exports.Promise = Promise;
//// [moduleInTypePosition1_1.js]
"use strict";
exports.__esModule = true;
var x = function (w1) { };
