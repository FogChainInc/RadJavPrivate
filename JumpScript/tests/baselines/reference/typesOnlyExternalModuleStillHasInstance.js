//// [tests/cases/conformance/externalModules/typesOnlyExternalModuleStillHasInstance.jump] ////

//// [foo_0.jump]
export interface Person {
	name: string;
	age: number;
}

export module M2 {
	export interface I2 {
		x: Person;
	}
}

//// [foo_1.jump]
import foo0 = require('./foo_0');
// Per 11.2.3, foo_0 should still be "instantiated", albeit with no members

var x: typeof foo0 = {};
var y: {M2: Object} = foo0;


//// [foo_0.js]
"use strict";
exports.__esModule = true;
//// [foo_1.js]
"use strict";
exports.__esModule = true;
var foo0 = require("./foo_0");
// Per 11.2.3, foo_0 should still be "instantiated", albeit with no members
var x = {};
var y = foo0;
