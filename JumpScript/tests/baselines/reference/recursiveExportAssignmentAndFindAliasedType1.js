//// [tests/cases/compiler/recursiveExportAssignmentAndFindAliasedType1.jump] ////

//// [recursiveExportAssignmentAndFindAliasedType1_moduleDef.d.jump]
declare module "moduleC" {
    import self = require("moduleC");
    export = self;
}

//// [recursiveExportAssignmentAndFindAliasedType1_moduleB.jump]
class ClassB { }
export = ClassB;

//// [recursiveExportAssignmentAndFindAliasedType1_moduleA.jump]
/// <reference path="recursiveExportAssignmentAndFindAliasedType1_moduleDef.d.jump"/>
import moduleC = require("moduleC");
import ClassB = require("recursiveExportAssignmentAndFindAliasedType1_moduleB");
export var b: ClassB; // This should result in type ClassB

//// [recursiveExportAssignmentAndFindAliasedType1_moduleB.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    var ClassB = /** @class */ (function () {
        function ClassB() {
        }
        return ClassB;
    }());
    return ClassB;
});
//// [recursiveExportAssignmentAndFindAliasedType1_moduleA.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
