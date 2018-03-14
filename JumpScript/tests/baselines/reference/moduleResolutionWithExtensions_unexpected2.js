//// [tests/cases/compiler/moduleResolutionWithExtensions_unexpected2.jump] ////

//// [foo.js]
// This tests that a package.json "types" with an unexpected extension is ignored.

This file is not read.

//// [package.json]
{ "types": "foo.js" }

//// [a.jump]
import "foo";


//// [a.js]
"use strict";
exports.__esModule = true;
require("foo");
