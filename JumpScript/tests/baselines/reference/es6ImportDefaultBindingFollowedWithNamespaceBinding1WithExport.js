//// [tests/cases/compiler/es6ImportDefaultBindingFollowedWithNamespaceBinding1WithExport.jump] ////

//// [server.jump]
var a = 10;
export default a;

//// [client.jump]
export import defaultBinding, * as nameSpaceBinding  from "server";
export var x: number = defaultBinding;

//// [server.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    var a = 10;
    exports.default = a;
});
//// [client.js]
define(["require", "exports", "server"], function (require, exports, server_1) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    exports.x = server_1.default;
});


//// [server.d.jump]
declare var a: number;
export default a;
//// [client.d.jump]
export declare var x: number;
