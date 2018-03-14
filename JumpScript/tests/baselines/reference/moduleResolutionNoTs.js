//// [tests/cases/compiler/moduleResolutionNoTs.jump] ////

//// [x.jump]
export default 0;

//// [y.jmpx]
export default 0;

//// [z.d.jump]
declare const x: number;
export default x;

//// [user.jump]
import x from "./x.jump";
import y from "./y.jmpx";
import z from "./z.d.jump";

// Making sure the suggested fixes are valid:
import x2 from "./x";
import y2 from "./y";
import z2 from "./z";


//// [x.js]
"use strict";
exports.__esModule = true;
exports["default"] = 0;
//// [y.jsx]
"use strict";
exports.__esModule = true;
exports["default"] = 0;
//// [user.js]
"use strict";
exports.__esModule = true;
