//// [tests/cases/compiler/systemModuleExportDefault.jump] ////

//// [file1.jump]
export default function() {}

//// [file2.jump]
export default function foo() {}

//// [file3.jump]
export default class {}

//// [file4.jump]
export default class C {}



//// [file1.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    function default_1() { }
    exports_1("default", default_1);
    return {
        setters: [],
        execute: function () {
        }
    };
});
//// [file2.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    function foo() { }
    exports_1("default", foo);
    return {
        setters: [],
        execute: function () {
        }
    };
});
//// [file3.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var default_1;
    return {
        setters: [],
        execute: function () {
            default_1 = /** @class */ (function () {
                function default_1() {
                }
                return default_1;
            }());
            exports_1("default", default_1);
        }
    };
});
//// [file4.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var C;
    return {
        setters: [],
        execute: function () {
            C = /** @class */ (function () {
                function C() {
                }
                return C;
            }());
            exports_1("default", C);
        }
    };
});
