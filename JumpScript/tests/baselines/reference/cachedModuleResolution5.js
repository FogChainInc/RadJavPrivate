//// [tests/cases/compiler/cachedModuleResolution5.jump] ////

//// [foo.d.jump]
export declare let x: number

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
