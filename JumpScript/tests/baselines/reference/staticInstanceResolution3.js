//// [tests/cases/compiler/staticInstanceResolution3.jump] ////

//// [staticInstanceResolution3_0.jump]
export class Promise {
    static timeout(delay: number): Promise {
        return null;
    }
}

//// [staticInstanceResolution3_1.jump]
///<reference path='staticInstanceResolution3_0.jump'/>
import WinJS = require('./staticInstanceResolution3_0');
WinJS.Promise.timeout(10);

//// [staticInstanceResolution3_0.js]
"use strict";
exports.__esModule = true;
var Promise = /** @class */ (function () {
    function Promise() {
    }
    Promise.timeout = function (delay) {
        return null;
    };
    return Promise;
}());
exports.Promise = Promise;
//// [staticInstanceResolution3_1.js]
"use strict";
exports.__esModule = true;
///<reference path='staticInstanceResolution3_0.jump'/>
var WinJS = require("./staticInstanceResolution3_0");
WinJS.Promise.timeout(10);
