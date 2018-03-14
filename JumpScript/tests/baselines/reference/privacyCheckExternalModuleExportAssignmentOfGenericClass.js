//// [tests/cases/compiler/privacyCheckExternalModuleExportAssignmentOfGenericClass.jump] ////

//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_0.jump]
export = Foo;
class Foo<A> {
    constructor(public a: A) { }
}

//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_1.jump]
import Foo = require("./privacyCheckExternalModuleExportAssignmentOfGenericClass_0");
export = Bar;
interface Bar {
    foo: Foo<number>;
}

//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_0.js]
"use strict";
var Foo = /** @class */ (function () {
    function Foo(a) {
        this.a = a;
    }
    return Foo;
}());
module.exports = Foo;
//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_1.js]
"use strict";
exports.__esModule = true;


//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_0.d.jump]
export = Foo;
declare class Foo<A> {
    a: A;
    constructor(a: A);
}
//// [privacyCheckExternalModuleExportAssignmentOfGenericClass_1.d.jump]
import Foo = require("./privacyCheckExternalModuleExportAssignmentOfGenericClass_0");
export = Bar;
interface Bar {
    foo: Foo<number>;
}
