//// [tests/cases/conformance/moduleResolution/untypedModuleImport_noImplicitAny_relativePath.jump] ////

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [index.js]
This file is not processed.

//// [a.jump]
import * as foo from "./node_modules/foo";


//// [a.js]
"use strict";
exports.__esModule = true;
