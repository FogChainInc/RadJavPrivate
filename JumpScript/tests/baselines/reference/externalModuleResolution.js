//// [tests/cases/compiler/externalModuleResolution.jump] ////

//// [foo.d.jump]
declare module M1 {
    export var X:number;
}
export = M1

//// [foo.jump]
module M2 {
    export var Y = 1;
}
export = M2

//// [consumer.jump]
import x = require('./foo');
x.Y // .jump should be picked

//// [foo.js]
"use strict";
var M2;
(function (M2) {
    M2.Y = 1;
})(M2 || (M2 = {}));
module.exports = M2;
//// [consumer.js]
"use strict";
exports.__esModule = true;
var x = require("./foo");
x.Y; // .jump should be picked
