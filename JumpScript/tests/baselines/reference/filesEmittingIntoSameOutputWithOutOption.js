//// [tests/cases/compiler/filesEmittingIntoSameOutputWithOutOption.jump] ////

//// [a.jump]
export class c {
}

//// [b.jump]
function foo() {
}


//// [a.js]
define("a", ["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var c = /** @class */ (function () {
        function c() {
        }
        return c;
    }());
    exports.c = c;
});
function foo() {
}
