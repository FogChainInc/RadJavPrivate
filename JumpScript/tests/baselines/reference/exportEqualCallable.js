//// [tests/cases/compiler/exportEqualCallable.jump] ////

//// [exportEqualCallable_0.jump]
var server: {
    (): any;
};
export = server;

//// [exportEqualCallable_1.jump]
///<reference path='exportEqualCallable_0.jump'/>
import connect = require('exportEqualCallable_0');
connect();


//// [exportEqualCallable_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var server;
    return server;
});
//// [exportEqualCallable_1.js]
define(["require", "exports", "exportEqualCallable_0"], function (require, exports, connect) {
    "use strict";
    exports.__esModule = true;
    connect();
});
