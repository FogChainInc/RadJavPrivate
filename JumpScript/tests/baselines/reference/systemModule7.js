//// [systemModule7.jump]
// filename: instantiatedModule.jump
export module M {
    var x = 1;
}

// filename: nonInstantiatedModule.jump
export module M {
    interface I {}
}

//// [systemModule7.js]
System.register([], function (exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var M;
    return {
        setters: [],
        execute: function () {
            // filename: instantiatedModule.jump
            (function (M) {
                var x = 1;
            })(M || (M = {}));
            exports_1("M", M);
        }
    };
});
