//// [tests/cases/conformance/ambient/ambientShorthand_reExport.jump] ////

//// [declarations.d.jump]
declare module "jquery";

//// [reExportX.jump]
export {x} from "jquery";

//// [reExportAll.jump]
export * from "jquery";

//// [reExportUser.jump]
import {x} from "./reExportX";
import * as $ from "./reExportAll";
// '$' is not callable, it is an object.
x($);


//// [reExportX.js]
"use strict";
exports.__esModule = true;
var jquery_1 = require("jquery");
exports.x = jquery_1.x;
//// [reExportAll.js]
"use strict";
function __export(m) {
    for (var p in m) if (!exports.hasOwnProperty(p)) exports[p] = m[p];
}
exports.__esModule = true;
__export(require("jquery"));
//// [reExportUser.js]
"use strict";
exports.__esModule = true;
var reExportX_1 = require("./reExportX");
var $ = require("./reExportAll");
// '$' is not callable, it is an object.
reExportX_1.x($);
