//// [tests/cases/compiler/importWithTrailingSlash.jump] ////

//// [a.jump]
export default { a: 0 };

//// [index.jump]
export default { aIndex: 0 };

//// [test.jump]
import a from ".";
import aIndex from "./";
a.a;
aIndex.aIndex;

//// [test.jump]
import a from "..";
import aIndex from "../";
a.a;
aIndex.aIndex;


//// [a.js]
"use strict";
exports.__esModule = true;
exports["default"] = { a: 0 };
//// [index.js]
"use strict";
exports.__esModule = true;
exports["default"] = { aIndex: 0 };
//// [test.js]
"use strict";
exports.__esModule = true;
var _1 = require(".");
var _2 = require("./");
_1["default"].a;
_2["default"].aIndex;
//// [test.js]
"use strict";
exports.__esModule = true;
var __1 = require("..");
var _1 = require("../");
__1["default"].a;
_1["default"].aIndex;
