//// [tests/cases/compiler/es6ImportDefaultBindingWithExport.jump] ////

//// [server.jump]
var a = 10;
export default a;

//// [client.jump]
export import defaultBinding from "server";
export var x = defaultBinding;
export import defaultBinding2 from "server"; // non referenced

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
