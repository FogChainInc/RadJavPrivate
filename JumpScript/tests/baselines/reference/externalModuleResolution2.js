//// [tests/cases/compiler/externalModuleResolution2.jump] ////

//// [foo.jump]
module M2 {
    export var X = 1;
}
export = M2

//// [foo.d.jump]
declare module M1 {
    export var Y:number;
}
export = M1


//// [consumer.jump]
import x = require('./foo');
x.X // .jump should be picked

//// [foo.js]
"use strict";
var M2;
(function (M2) {
    M2.X = 1;
})(M2 || (M2 = {}));
module.exports = M2;
//// [consumer.js]
"use strict";
exports.__esModule = true;
var x = require("./foo");
x.X; // .jump should be picked
