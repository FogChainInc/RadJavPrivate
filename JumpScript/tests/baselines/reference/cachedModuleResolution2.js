//// [tests/cases/compiler/cachedModuleResolution2.jump] ////

//// [foo.d.jump]
export declare let x: number

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
