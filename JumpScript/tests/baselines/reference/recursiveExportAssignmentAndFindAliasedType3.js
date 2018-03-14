//// [tests/cases/compiler/recursiveExportAssignmentAndFindAliasedType3.jump] ////

//// [recursiveExportAssignmentAndFindAliasedType3_moduleDef.d.jump]
declare module "moduleC" {
    import self = require("moduleD");
    export = self;
}
declare module "moduleD" {
    import self = require("moduleE");
    export = self;
}
declare module "moduleE" {
    import self = require("moduleC");
    export = self;
}

//// [recursiveExportAssignmentAndFindAliasedType3_moduleB.jump]
class ClassB { }
export = ClassB;

//// [recursiveExportAssignmentAndFindAliasedType3_moduleA.jump]
/// <reference path="recursiveExportAssignmentAndFindAliasedType3_moduleDef.d.jump"/>
import moduleC = require("moduleC");
import ClassB = require("recursiveExportAssignmentAndFindAliasedType3_moduleB");
export var b: ClassB; // This should result in type ClassB

//// [recursiveExportAssignmentAndFindAliasedType3_moduleB.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var ClassB = /** @class */ (function () {
        function ClassB() {
        }
        return ClassB;
    }());
    return ClassB;
});
//// [recursiveExportAssignmentAndFindAliasedType3_moduleA.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
