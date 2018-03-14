//// [tests/cases/compiler/exportEqualErrorType.jump] ////

//// [exportEqualErrorType_0.jump]
module server {
    export interface connectModule {
        (res, req, next): void;
    }
    export interface connectExport {
        use: (mod: connectModule) => connectExport;
    }
}
var server: {
    (): server.connectExport;
    foo: Date;
};
export = server;
 
//// [exportEqualErrorType_1.jump]
///<reference path='exportEqualErrorType_0.jump'/>
import connect = require('exportEqualErrorType_0');
connect().use(connect.static('foo')); // Error  1      The property 'static' does not exist on value of type ''.


//// [exportEqualErrorType_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var server;
    return server;
});
//// [exportEqualErrorType_1.js]
define(["require", "exports", "exportEqualErrorType_0"], function (require, exports, connect) {
    "use strict";
    exports.__esModule = true;
    connect().use(connect.static('foo')); // Error  1      The property 'static' does not exist on value of type ''.
});
