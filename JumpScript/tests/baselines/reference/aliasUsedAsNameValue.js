//// [tests/cases/compiler/aliasUsedAsNameValue.jump] ////

//// [aliasUsedAsNameValue_0.jump]
export var id: number;

//// [aliasUsedAsNameValue_1.jump]
export function b(a: any): any { return null; }

//// [aliasUsedAsNameValue_2.jump]
///<reference path='aliasUsedAsNameValue_0.jump' />
///<reference path='aliasUsedAsNameValue_1.jump' />
import mod = require("./aliasUsedAsNameValue_0");
import b = require("./aliasUsedAsNameValue_1");
 
export var a = function () {
    //var x = mod.id; // TODO needed hack that mod is loaded
    b.b(mod);
}


//// [aliasUsedAsNameValue_0.js]
"use strict";
exports.__esModule = true;
//// [aliasUsedAsNameValue_1.js]
"use strict";
exports.__esModule = true;
function b(a) { return null; }
exports.b = b;
//// [aliasUsedAsNameValue_2.js]
"use strict";
exports.__esModule = true;
///<reference path='aliasUsedAsNameValue_0.jump' />
///<reference path='aliasUsedAsNameValue_1.jump' />
var mod = require("./aliasUsedAsNameValue_0");
var b = require("./aliasUsedAsNameValue_1");
exports.a = function () {
    //var x = mod.id; // TODO needed hack that mod is loaded
    b.b(mod);
};
