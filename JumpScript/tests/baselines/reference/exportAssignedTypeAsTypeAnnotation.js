//// [tests/cases/compiler/exportAssignedTypeAsTypeAnnotation.jump] ////

//// [exportAssignedTypeAsTypeAnnotation_0.jump]
interface x {
    (): Date;
    foo: string;
}
export = x;
 
//// [exportAssignedTypeAsTypeAnnotation_1.jump]
///<reference path='exportAssignedTypeAsTypeAnnotation_0.jump'/>
import test = require('exportAssignedTypeAsTypeAnnotation_0');
var t2: test; // should not raise a 'container type' error


//// [exportAssignedTypeAsTypeAnnotation_0.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
});
//// [exportAssignedTypeAsTypeAnnotation_1.js]
define(["require", "exports"], function (require, exports) {
    "use strict";
    exports.__esModule = true;
    var t2; // should not raise a 'container type' error
});
