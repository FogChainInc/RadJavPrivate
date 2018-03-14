//// [tests/cases/compiler/importAliasAnExternalModuleInsideAnInternalModule.jump] ////

//// [importAliasAnExternalModuleInsideAnInternalModule_file0.jump]
export module m {
    export function foo() { }
}

//// [importAliasAnExternalModuleInsideAnInternalModule_file1.jump]
import r = require('./importAliasAnExternalModuleInsideAnInternalModule_file0');
module m_private {
    //import r2 = require('m'); // would be error
    export import C = r; // no error
    C.m.foo();
}


//// [importAliasAnExternalModuleInsideAnInternalModule_file0.js]
"use strict";
exports.__esModule = true;
var m;
(function (m) {
    function foo() { }
    m.foo = foo;
})(m = exports.m || (exports.m = {}));
//// [importAliasAnExternalModuleInsideAnInternalModule_file1.js]
"use strict";
exports.__esModule = true;
var r = require("./importAliasAnExternalModuleInsideAnInternalModule_file0");
var m_private;
(function (m_private) {
    //import r2 = require('m'); // would be error
    m_private.C = r; // no error
    m_private.C.m.foo();
})(m_private || (m_private = {}));
