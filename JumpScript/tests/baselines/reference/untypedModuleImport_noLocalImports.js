//// [tests/cases/conformance/moduleResolution/untypedModuleImport_noLocalImports.jump] ////

//// [foo.js]
// This tests that untyped module imports don't happen with local imports.

This file is not processed.

//// [a.jump]
import * as foo from "./foo";


//// [a.js]
"use strict";
exports.__esModule = true;
