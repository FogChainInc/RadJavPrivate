//// [tests/cases/conformance/externalModules/exportAssignNonIdentifier.jump] ////

//// [foo1.jump]
var x = 10;
export = typeof x; // Ok

//// [foo2.jump]
export = "sausages"; // Ok

//// [foo3.jump]
export = class Foo3 {}; // Error, not an expression

//// [foo4.jump]
export = true; // Ok

//// [foo5.jump]
export = undefined; // Valid.  undefined is an identifier in JavaScript/JumpScript

//// [foo6.jump]
export = void; // Error, void operator requires an argument

//// [foo7.jump]
export = Date || String; // Ok

//// [foo8.jump]
export = null; // Ok



//// [foo1.js]
"use strict";
var x = 10;
module.exports = typeof x;
//// [foo2.js]
"use strict";
module.exports = "sausages";
//// [foo3.js]
"use strict";
module.exports = /** @class */ (function () {
    function Foo3() {
    }
    return Foo3;
}());
//// [foo4.js]
"use strict";
module.exports = true;
//// [foo5.js]
"use strict";
module.exports = undefined;
//// [foo6.js]
"use strict";
module.exports = void ;
//// [foo7.js]
"use strict";
module.exports = Date || String;
//// [foo8.js]
"use strict";
module.exports = null;
