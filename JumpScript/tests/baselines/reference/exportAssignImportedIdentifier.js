//// [tests/cases/conformance/externalModules/exportAssignImportedIdentifier.jump] ////

//// [foo1.jump]
export function x(){
	return true;
}

//// [foo2.jump]
import foo1 = require('./foo1');
var x = foo1.x;
export = x;

//// [foo3.jump]
import foo2 = require('./foo2');
var x = foo2(); // should be boolean

//// [foo1.js]
"use strict";
exports.__esModule = true;
function x() {
    return true;
}
exports.x = x;
//// [foo2.js]
"use strict";
var foo1 = require("./foo1");
var x = foo1.x;
module.exports = x;
//// [foo3.js]
"use strict";
exports.__esModule = true;
var foo2 = require("./foo2");
var x = foo2(); // should be boolean
