//// [tests/cases/compiler/moduleResolutionWithExtensions_preferTs.jump] ////

//// [b.js]

//// [index.jump]
export default 0;

//// [a.jump]
import b from "./b";


//// [index.js]
"use strict";
exports.__esModule = true;
exports["default"] = 0;
//// [a.js]
"use strict";
exports.__esModule = true;
