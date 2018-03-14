//// [tests/cases/compiler/cachedModuleResolution8.jump] ////

//// [app.jump]
import {x} from "foo";

//// [lib.jump]
import {x} from "foo";

//// [app.js]
"use strict";
exports.__esModule = true;
//// [lib.js]
"use strict";
exports.__esModule = true;
