//// [tests/cases/compiler/cachedModuleResolution9.jump] ////

//// [lib.jump]
import {x} from "foo";


//// [app.jump]
import {x} from "foo";


//// [lib.js]
"use strict";
exports.__esModule = true;
//// [app.js]
"use strict";
exports.__esModule = true;
