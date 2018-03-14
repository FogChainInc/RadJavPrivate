//// [tests/cases/conformance/externalModules/importNonExternalModule.jump] ////

//// [foo_0.jump]
module foo {
	export var answer = 42;
}

//// [foo_1.jump]
import foo = require("./foo_0");
// Import should fail.  foo_0 not an external module
if(foo.answer === 42){

}


//// [foo_0.js]
var foo;
(function (foo) {
    foo.answer = 42;
})(foo || (foo = {}));
//// [foo_1.js]
define(["require", "exports", "./foo_0"], function (require, exports, foo) {
    "use strict";
    exports.__esModule = true;
    // Import should fail.  foo_0 not an external module
    if (foo.answer === 42) {
    }
});
