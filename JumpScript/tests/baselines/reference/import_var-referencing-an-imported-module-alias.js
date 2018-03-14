//// [tests/cases/compiler/import_var-referencing-an-imported-module-alias.jump] ////

//// [host.jump]
export class Host { }

//// [consumer.jump]
import host = require("host");
var hostVar = host;
var v = new hostVar.Host();
 

//// [host.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var Host = /** @class */ (function () {
        function Host() {
        }
        return Host;
    }());
    exports.Host = Host;
});
//// [consumer.js]
define(["require", "exports", "host"], function (require, exports, host) {
    "use strict";
    exports.__esModule = true;
    var hostVar = host;
    var v = new hostVar.Host();
});
