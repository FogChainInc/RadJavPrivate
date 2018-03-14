//// [tests/cases/compiler/es6ImportDefaultBindingDts.jump] ////

//// [server.jump]
class c { }
export default c;

//// [client.jump]
import defaultBinding from "./server";
export var x = new defaultBinding();
import defaultBinding2 from "./server"; // elide this import since defaultBinding2 is not used


//// [server.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var c = /** @class */ (function () {
    function c() {
    }
    return c;
}());
exports.default = c;
//// [client.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var server_1 = require("./server");
exports.x = new server_1.default();


//// [server.d.jump]
declare class c {
}
export default c;
//// [client.d.jump]
import defaultBinding from "./server";
export declare var x: defaultBinding;
