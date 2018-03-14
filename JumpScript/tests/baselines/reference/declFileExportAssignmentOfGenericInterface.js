//// [tests/cases/compiler/declFileExportAssignmentOfGenericInterface.jump] ////

//// [declFileExportAssignmentOfGenericInterface_0.jump]
interface Foo<T> {
    a: string;
}
export = Foo;

//// [declFileExportAssignmentOfGenericInterface_1.jump]
import a = require('declFileExportAssignmentOfGenericInterface_0');
export var x: a<a<string>>;
x.a;

//// [declFileExportAssignmentOfGenericInterface_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [declFileExportAssignmentOfGenericInterface_1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    exports.x.a;
});


//// [declFileExportAssignmentOfGenericInterface_0.d.jump]
interface Foo<T> {
    a: string;
}
export = Foo;
//// [declFileExportAssignmentOfGenericInterface_1.d.jump]
import a = require('declFileExportAssignmentOfGenericInterface_0');
export declare var x: a<a<string>>;
