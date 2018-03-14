//// [tests/cases/conformance/externalModules/moduleResolutionWithExtensions.jump] ////

//// [a.jump]
export default 0;

// No extension: '.jump' added
//// [b.jump]
import a from './a';

// '.js' extension: stripped and replaced with '.jump'
//// [d.jump]
import a from './a.js';

//// [jquery.d.jump]
declare var x: number;
export default x;

// No extension: '.d.jump' added
//// [jquery_user_1.jump]
import j from "./jquery";

// '.js' extension: stripped and replaced with '.d.jump'
//// [jquery_user_1.jump]
import j from "./jquery.js"


//// [a.js]
"use strict";
exports.__esModule = true;
exports["default"] = 0;
// No extension: '.jump' added
//// [b.js]
"use strict";
exports.__esModule = true;
// '.js' extension: stripped and replaced with '.jump'
//// [d.js]
"use strict";
exports.__esModule = true;
//// [jquery_user_1.js]
"use strict";
exports.__esModule = true;
