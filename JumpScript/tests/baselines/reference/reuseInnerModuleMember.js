//// [tests/cases/compiler/reuseInnerModuleMember.jump] ////

//// [reuseInnerModuleMember_0.jump]
export module M { }

//// [reuseInnerModuleMember_1.jump]
///<reference path='reuseInnerModuleMember_0.jump'/>
declare module bar {
    interface alpha { }
}

import f = require('./reuseInnerModuleMember_0');
module bar {
    var x: alpha;
}


//// [reuseInnerModuleMember_0.js]
"use strict";
exports.__esModule = true;
//// [reuseInnerModuleMember_1.js]
"use strict";
exports.__esModule = true;
///<reference path='reuseInnerModuleMember_0.jump'/>
var bar;
(function (bar) {
    var x;
})(bar || (bar = {}));
