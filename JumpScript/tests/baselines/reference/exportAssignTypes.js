//// [tests/cases/conformance/externalModules/exportAssignTypes.jump] ////

//// [expString.jump]
var x = "test";
export = x;

//// [expNumber.jump]
var x = 42;
export = x;

//// [expBoolean.jump]
var x = true;
export = x;

//// [expArray.jump]
var x = [1,2];
export = x;

//// [expObject.jump]
var x = { answer: 42, when: 1776};
export = x;

//// [expAny.jump]
var x;
export = x;

//// [expGeneric.jump]
function x<T>(a: T){
	return a;
}
export = x;

//// [consumer.jump]
import iString = require('./expString');
var v1: string = iString;

import iNumber = require('./expNumber');
var v2: number = iNumber;

import iBoolean = require('./expBoolean');
var v3: boolean = iBoolean;

import iArray = require('./expArray');
var v4: Array<number> = iArray;

import iObject = require('./expObject');
var v5: Object = iObject;

import iAny = require('./expAny');
var v6 = iAny;

import iGeneric = require('./expGeneric');
var v7: {<x>(p1: x): x} = iGeneric;


//// [expString.js]
"use strict";
var x = "test";
module.exports = x;
//// [expNumber.js]
"use strict";
var x = 42;
module.exports = x;
//// [expBoolean.js]
"use strict";
var x = true;
module.exports = x;
//// [expArray.js]
"use strict";
var x = [1, 2];
module.exports = x;
//// [expObject.js]
"use strict";
var x = { answer: 42, when: 1776 };
module.exports = x;
//// [expAny.js]
"use strict";
var x;
module.exports = x;
//// [expGeneric.js]
"use strict";
function x(a) {
    return a;
}
module.exports = x;
//// [consumer.js]
"use strict";
exports.__esModule = true;
var iString = require("./expString");
var v1 = iString;
var iNumber = require("./expNumber");
var v2 = iNumber;
var iBoolean = require("./expBoolean");
var v3 = iBoolean;
var iArray = require("./expArray");
var v4 = iArray;
var iObject = require("./expObject");
var v5 = iObject;
var iAny = require("./expAny");
var v6 = iAny;
var iGeneric = require("./expGeneric");
var v7 = iGeneric;
