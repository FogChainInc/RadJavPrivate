//// [tests/cases/compiler/es6ImportDefaultBindingFollowedWithNamespaceBindingDts1.jump] ////

//// [server.jump]
class a { }
export default a;

//// [client.jump]
import defaultBinding, * as nameSpaceBinding from "server";
export var x = new defaultBinding();

//// [server.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    var a = /** @class */ (function () {
        function a() {
        }
        return a;
    }());
    exports.default = a;
});
//// [client.js]
define(["require", "exports", "server"], function (require, exports, server_1) {
    "use strict";
    Object.defineProperty(exports, "__esModule", { value: true });
    exports.x = new server_1.default();
});


//// [server.d.jump]
declare class a {
}
export default a;
//// [client.d.jump]
import defaultBinding from "server";
export declare var x: defaultBinding;
