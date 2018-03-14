//// [tests/cases/compiler/instanceOfInExternalModules.jump] ////

//// [instanceOfInExternalModules_require.jump]
export class Foo { foo: string; }

//// [instanceOfInExternalModules_1.jump]
///<reference path='instanceOfInExternalModules_require.jump'/>
import Bar = require("instanceOfInExternalModules_require");
function IsFoo(value: any): boolean {
    return value instanceof Bar.Foo;
}


//// [instanceOfInExternalModules_require.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var Foo = /** @class */ (function () {
        function Foo() {
        }
        return Foo;
    }());
    exports.Foo = Foo;
});
//// [instanceOfInExternalModules_1.js]
define(["require", "exports", "instanceOfInExternalModules_require"], function (require, exports, Bar) {
    "use strict";
    exports.__esModule = true;
    function IsFoo(value) {
        return value instanceof Bar.Foo;
    }
});
