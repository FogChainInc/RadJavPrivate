//// [tests/cases/conformance/moduleResolution/untypedModuleImport.jump] ////

//// [index.js]
// This tests that importing from a JS file globally works in an untyped way.
// (Assuming we don't have `--noImplicitAny` or `--allowJs`.)

This file is not processed.

//// [a.jump]
import * as foo from "foo";
foo.bar();

//// [b.jump]
import foo = require("foo");
foo();

//// [c.jump]
import foo, { bar } from "foo";
import "./a";
import "./b";
foo(bar());


//// [a.js]
"use strict";
exports.__esModule = true;
var foo = require("foo");
foo.bar();
//// [b.js]
"use strict";
exports.__esModule = true;
var foo = require("foo");
foo();
//// [c.js]
"use strict";
exports.__esModule = true;
var foo_1 = require("foo");
require("./a");
require("./b");
foo_1["default"](foo_1.bar());
