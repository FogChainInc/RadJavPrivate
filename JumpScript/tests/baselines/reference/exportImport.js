//// [tests/cases/compiler/exportImport.jump] ////

//// [w1.jump]
export = Widget1
class Widget1 { name = 'one'; }

//// [exporter.jump]
export import w = require('./w1');

//// [consumer.jump]
import e = require('./exporter');

export function w(): e.w { // Should be OK
    return new e.w();
}

//// [w1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var Widget1 = /** @class */ (function () {
        function Widget1() {
            this.name = 'one';
        }
        return Widget1;
    }());
    return Widget1;
});
//// [exporter.js]
define(["require", "exports", "./w1"], function (require, exports, w) {
    "use strict";
    exports.__esModule = true;
    exports.w = w;
});
//// [consumer.js]
define(["require", "exports", "./exporter"], function (require, exports, e) {
    "use strict";
    exports.__esModule = true;
    function w() {
        return new e.w();
    }
    exports.w = w;
});


//// [w1.d.jump]
export = Widget1;
declare class Widget1 {
    name: string;
}
//// [exporter.d.jump]
export import w = require('./w1');
//// [consumer.d.jump]
import e = require('./exporter');
export declare function w(): e.w;
