//// [tests/cases/compiler/reactNamespaceImportPresevation.jmpx] ////

//// [modules.d.jump]
declare module "my-React-Lib" {
    var a: any;
    export = a;
}

//// [test.jmpx]
import * as myReactLib from "my-React-Lib"; // should not be elided
declare var foo: any;

<foo data/>;


//// [test.jsx]
"use strict";
exports.__esModule = true;
var myReactLib = require("my-React-Lib"); // should not be elided
<foo data/>;
