//// [tests/cases/compiler/exportImportNonInstantiatedModule2.jump] ////

//// [w1.jump]
export = Widget1
interface Widget1 { name: string; }

//// [exporter.jump]
export import w = require('./w1');

//// [consumer.jump]
import e = require('./exporter');

export function w(): e.w { // Should be OK
    return {name: 'value' };
}

//// [w1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [exporter.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [consumer.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    function w() {
        return { name: 'value' };
    }
    exports.w = w;
});


//// [w1.d.jump]
export = Widget1;
interface Widget1 {
    name: string;
}
//// [exporter.d.jump]
export import w = require('./w1');
//// [consumer.d.jump]
import e = require('./exporter');
export declare function w(): e.w;
